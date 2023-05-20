#include "NitEditorPCH.h"
#include "EditorCameraComponent.h"

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace Nit;
    
    registration::class_<EditorCameraComponent>("EditorCameraComponent")
        .property("CameraData", &EditorCameraComponent::CameraData)
        .property("MoveSpeed", &EditorCameraComponent::MoveSpeed)
        .property("CanMove", &EditorCameraComponent::bCanMove);
}