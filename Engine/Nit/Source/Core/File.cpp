#include "File.h"
#include "Window\Window.h"
#include <GLFW\glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW\glfw3native.h"

//TODO: Make this platform independent
namespace Nit
{
    static GLFWwindow* m_WindowHandler = nullptr;

    bool File::m_bInitialized = false;

    void File::Start(const UniquePtr<Window>& window)
    {
        m_WindowHandler = static_cast<GLFWwindow*>(window->GetHandler());
        m_bInitialized = true;
    }

    String File::OpenFile(const String& filter)
    {
        NIT_CHECK(m_bInitialized, "File system not initialized!");
        if (!m_bInitialized)
            return {};

        OPENFILENAMEA ofn;
        CHAR szFile[260] = { 0 };
        CHAR currentDir[256] = { 0 };
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window(m_WindowHandler);
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        if (GetCurrentDirectoryA(256, currentDir))
            ofn.lpstrInitialDir = currentDir;
        ofn.lpstrFilter = filter.c_str();
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

        // Sets the default extension by extracting it from the filter
        ofn.lpstrDefExt = strchr(filter.c_str(), '\0') + 1;

        if (GetSaveFileNameA(&ofn) == TRUE)
            return ofn.lpstrFile;

        return {};
    }

    String File::SaveFile(const String& filter)
    {
        NIT_CHECK(m_bInitialized, "File system not initialized!");
        if (!m_bInitialized)
            return {};

        OPENFILENAMEA ofn;
        CHAR szFile[260] = { 0 };
        CHAR currentDir[256] = { 0 };
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window(m_WindowHandler);
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        if (GetCurrentDirectoryA(256, currentDir))
            ofn.lpstrInitialDir = currentDir;
        ofn.lpstrFilter = filter.c_str();
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

        // Sets the default extension by extracting it from the filter
        ofn.lpstrDefExt = strchr(filter.c_str(), '\0') + 1;

        if (GetSaveFileNameA(&ofn) == TRUE)
            return ofn.lpstrFile;

        return {};
    }

}