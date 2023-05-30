#pragma once

// Core
#include "Core/Engine.h"
#include "Core/EngineLayer.h"
#include "Core/LayerStack.h"
#include "Core/Asset/AssetManager.h"
#include "Core/Asset/AssetLink.h"
#include "Core/Serialization.h"

// Rendering

#include "Rendering/Data/BufferElement.h"
#include "Rendering/Data/BufferLayout.h"
#include "Rendering/Data/IndexBuffer.h"
#include "Rendering/Data/Shader.h"
#include "Rendering/Data/Texture2D.h"
#include "Rendering/Data/VertexArray.h"
#include "Rendering/Data/VertexBuffer.h"

#include "Rendering/RenderCommand.h"
#include "Rendering/RenderCommandQueue.h"
#include "Rendering/Renderer2D.h"
#include "Rendering/RendererAPI.h"
#include "Rendering/RenderingContext.h"
#include "Rendering/RawShaderStrings.h"
#include "Rendering/Text/Font.h"
#include "Rendering/Camera.h"

// ImGui
#ifdef NIT_IMGUI
#include "ImGui/ImGuiRenderer.h"
#include "ImGui/ImGuiHelpers.h"
#endif

// Audio
#include "Audio/AudioClip.h"
#include "Audio/AudioManager.h"
#include "Audio/AudioSource.h"

// Window

#include "Window/Input/Input.h"
#include "Window/Input/KeyCodes.h"
#include "Window/Window.h"
#include "Window/WindowEvents.h"

// Scene

#include "Scene/Actor.h"
#include "Scene/Scene.h"
#include "Scene/World.h"
#include "Scene/Components/ActorComponent.h"
#include "Scene/Components/TransformComponent.h"
#include "Scene/Components/SpriteComponent.h"
#include "Scene/Components/CameraComponent.h"
#include "Scene/Components/TextComponent.h"
#include "Scene/Components/AudioSourceComponent.h"
#include "Scene/Components/DetailsComponent.h"

#include "Scene/Systems/SceneSystem.h"
#include "Scene/Systems/SpriteSystem.h"
#include "Scene/Systems/CameraSystem.h"
#include "Scene/Systems/TextSystem.h"
#include "Scene/Systems/AudioSystem.h"