#include "EditorCameraControllerComponent.h"
#include "TransformComponent.h"
#include "Entity/Entity.h"
#include "Window/Window.h"
#include "Core/Engine.h"
#include "ComponentReflection.h"

namespace Nit
{
    NIT_FORCE_LINK_IMPL(EditorCameraControllerComponent)
}

RTTR_REGISTRATION
{
    using namespace Nit;

    rttr::registration::class_<EditorCameraControllerComponent>("EditorCameraControllerComponent")
        .constructor<>()(rttr::policy::ctor::as_object)
        .property("ZoomStep", &EditorCameraControllerComponent::ZoomStep)
        .property("ZoomSpeed", &EditorCameraControllerComponent::ZoomSpeed)
        .property("MoveSpeed", &EditorCameraControllerComponent::MoveSpeed);

    ComponentReflection::RegisterComponentType<EditorCameraControllerComponent>();
}