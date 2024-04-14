#include "Window.h"
#include "NitRender/Platform/Windows/WindowsWindow.h"

namespace Nit
{
    TUniquePtr<CWindow> CWindow::Create(const CConfiguration& config)
    {
        return CreateUniquePtr<CWindowsWindow>(config);
    }
}