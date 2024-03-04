#include "Window.h"
#include "Platform\WindowsWindow.h"

namespace Nit
{
    UniquePtr<Window> Window::Create(const Configuration& config)
    {
        return CreateUniquePtr<WindowsWindow>(config);
    }
}