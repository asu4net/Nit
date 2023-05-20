#pragma once

#include <Windows.h>

#include <iostream>
#include <cstdio>
#include <algorithm>
#include <functional>
#include <memory>
#include <thread>
#include <utility>
#include <cmath>
#include <random>
#include <filesystem>

#include <string>
#include <sstream>
#include <fstream>
#include <array>
#include <vector>
#include <map>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/common.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/matrix_decompose.hpp"

#include <rttr/registration_friend.h>
#include <rttr/registration>
#include <rttr/type>

#ifdef NIT_IMGUI
#include <imgui.h>
#include <ImGuizmo.h>
#include <imgui_internal.h>
#include "ImGui/Implementation/imgui_impl_glfw.h"
#include "ImGui/Implementation/imgui_impl_opengl3.h"
#endif

#include <entt.hpp>

#include "Math/Math.h"

#include "Core/Enums.h"
#include "Core/Delegates/Delegate.h"
#include "Core/Delegates/MulticastDelegate.h"
#include "Core/Base.h"
#include "Core/Singleton.h"
#include "Core/Id.h"