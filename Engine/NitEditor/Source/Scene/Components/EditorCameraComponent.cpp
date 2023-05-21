#include "NitEditorPCH.h"
#include "EditorCameraComponent.h"

namespace Nit { NIT_FORCE_LINK_IMPL(EditorCameraComponent) }

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace Nit;
    
    registration::class_<EditorCameraComponent>("EditorCameraComponent")
        .property("CameraData", &EditorCameraComponent::CameraData)
        .property("MoveSpeed", &EditorCameraComponent::MoveSpeed)
        .property("CanMove", &EditorCameraComponent::bCanMove);
}