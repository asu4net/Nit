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
#include "Rendering/Camera.h"
#include "Rendering/RawShaderStrings.h"
#include "Rendering/Text/Font.h"

// ImGui
#ifdef NIT_IMGUI
#include "ImGui/ImGuiRenderer.h"
#include "ImGui/ImGuiWidget.h"
#include "ImGui/Widgets/Vector3Widget.h"
#endif

// Audio
#include "Audio/AudioBuffer.h"
#include "Audio/AudioManager.h"
#include "Audio/AudioSource.h"

// Window

#include "Window/Input/Input.h"
#include "Window/Input/KeyCodes.h"
#include "Window/Window.h"
#include "Window/WindowEvents.h"