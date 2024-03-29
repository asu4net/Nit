#include "FamilySystem.h"
#include "Component/FamilyComponent.h"
#include "Component/TransformComponent.h"
#include "Core/Engine.h"

// TODO: Bounding box. Parent should be the center.

namespace Nit::FamilySystem
{
    Matrix4 GetMatrix(Entity entity)
    {
        auto& childTransform = entity.GetTransform();
        const Matrix4 childMatrix = childTransform.GetMatrix();
        
        if (!entity.Has<FamilyComponent>())
        {
            return childMatrix;
        }

        auto& familyComponent = entity.Get<FamilyComponent>();
        EntityRef parentRef = familyComponent.ParentRef;
        
        if (!parentRef.IsValid())
        {
            return childMatrix;
        }

        Entity parent = parentRef.Get();

        if (parent == entity)
        {
            return childMatrix;
        }
        
        auto& parentTransform = parent.GetTransform();
        const Vector3 parentPosition = parentTransform.Position;
        const Vector3 parentRotation = parentTransform.Rotation;

        Vector3 childPosition = childTransform.Position + parentPosition;
        
        childPosition = Vector2::RotateAround(
            parentPosition,   // pivot
            parentRotation.z, // angle
            childPosition     // point
        );
        const Vector3 childRotation = childTransform.Rotation + parentRotation;
        const Vector3 childScale = Vector3::Multiply(childTransform.Scale, parentTransform.Scale);        
        return Matrix4::CreateTransform(childPosition, childRotation);
    }

    void ReconcileTransformWithParent(Entity entity)
    {
        NIT_CHECK(!entity.Has<FamilyComponent>(), "Family component required!");
        TransformComponent& transform = entity.GetTransform();
        FamilyComponent& family = entity.Get<FamilyComponent>();
        //ReconcileTransformWithParent(family, transform);
    }

    void ReconcileTransformWithParent(FamilyComponent& familyComponent, TransformComponent& transform)
    {
        if (!familyComponent.ParentRef.IsValid() || familyComponent.ParentRef == familyComponent.PrevParentRef)
        {
            return;
        }

        TransformComponent& parentTransform = familyComponent.ParentRef.Get().GetTransform();
        transform.Position -= parentTransform.Position;

        transform.Rotation -= parentTransform.Rotation;
        
        transform.Scale.x  /= parentTransform.Scale.x;
        transform.Scale.y  /= parentTransform.Scale.y;
        transform.Scale.z  /= parentTransform.Scale.z;
        
        familyComponent.PrevParentRef = EntityRef(familyComponent.ParentRef);
    }

    void OnCreate();
    void OnFamilyComponentAdded(Registry&, RawEntity entity);
    void OnFamilyComponentRemoved(Registry&, RawEntity entity);
    void OnPreDrawPrimitives();
    
    void Register()
    {
        Engine::CreateSystem("FamilySystem", FamilyExecutionOrder, ExecutionContext::Always);
        Engine::SetSystemCallback(SystemStage::Create, &OnCreate);
        Engine::SetSystemCallback(SystemStage::PreDrawPrimitives, &OnPreDrawPrimitives);
    }

    void OnCreate()
    {
        World::GetRegistry().on_construct<FamilyComponent>().connect<&OnFamilyComponentAdded>();
        World::GetRegistry().on_destroy<FamilyComponent>().connect<&OnFamilyComponentRemoved>();
    }

    void OnFamilyComponentAdded(Registry&, RawEntity entity)
    {
        Entity spriteEntity = entity;
        FamilyComponent& spriteComponent = spriteEntity.Get<FamilyComponent>();
        spriteComponent.ParentRef.Retarget();
        // retarget child as well
    }

    void OnFamilyComponentRemoved(Registry&, RawEntity entity)
    {
        Entity spriteEntity = entity;
        FamilyComponent& spriteComponent = spriteEntity.Get<FamilyComponent>();
    }

    void OnPreDrawPrimitives()
    {
        const auto view = World::GetRegistry().view<FamilyComponent, TransformComponent>();

        view.each([&](RawEntity rawEntity, FamilyComponent& family, TransformComponent& transform) {
            ReconcileTransformWithParent(family, transform);
        });
        
    }
}
