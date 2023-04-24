#include "Time.h"
#include <glfw/glfw3.h>

namespace Nit
{
    double Time::GetTime()
    {
        return glfwGetTime();
    }
}