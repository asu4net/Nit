#pragma once

// Core
#include "Core/Game.h"
#include "Core/Delegates/Delegate.h"
#include "Core/Delegates/MulticastDelegate.h"
#include "Core/Utility/Misc.h"
#include "Core/Utility/Singleton.h"
#include "Core/Time.h"
#include "Core/GameLayer.h"
#include "Core/LayerStack.h"

// Math

#include "Math/Defaults.h"

// Rendering

#include "Rendering/Data/BufferElement.h"
#include "Rendering/Data/BufferLayout.h"
#include "Rendering/Data/IndexBuffer.h"
#include "Rendering/Data/Shader.h"
#include "Rendering/Data/Texture.h"
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

// Window

#include "Window/Input/Input.h"
#include "Window/Input/KeyCodes.h"
#include "Window/Window.h"
#include "Window/WindowEvents.h"