#include "Window.h"
#include "Platform\Windows\WindowsWindow.h"

namespace Nit::Render
{
    UniquePtr<Window> Window::Create(const Configuration& config)
    {
        return CreateUniquePtr<WindowsWindow>(config);
    }
}