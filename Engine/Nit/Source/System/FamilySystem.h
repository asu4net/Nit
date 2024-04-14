#pragma once
#include "Component/FamilyComponent.h"
#include "Component/TransformComponent.h"
#include "Entity/Entity.h"

namespace Nit::FamilySystem
{
    CMatrix4 GetMatrix(Entity entity);

    void ReconcileTransformWithParent(Entity entity);
    void ReconcileTransformWithParent(FamilyComponent& familyComponent, TransformComponent& transform);

    void Register();
}