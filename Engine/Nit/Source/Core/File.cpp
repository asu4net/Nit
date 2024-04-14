#include "File.h"
#include "Window\Window.h"
#include <GLFW\glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW\glfw3native.h"
#include <ShlObj_core.h>

//TODO: Make this platform independent
namespace Nit
{
    static GLFWwindow* m_WindowHandler = nullptr;

    bool File::m_bInitialized = false;

    void File::Init(const TUniquePtr<CWindow>& window)
    {
        m_WindowHandler = static_cast<GLFWwindow*>(window->GetHandler());
        m_bInitialized = true;
    }

    TString File::OpenFile(const TString& filter)
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

    TString File::SaveFile(const TString& filter)
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

    BOOL CALLBACK BrowseCallbackProcW_enum(HWND hWndChild, LPARAM lParam)
    {
        wchar_t buf[255];
        GetClassNameW(hWndChild, buf, sizeof(buf));
        if (wcscmp(buf, L"SysTreeView32") == 0)
        {
            HTREEITEM hNode = TreeView_GetSelection(hWndChild);
            TreeView_EnsureVisible(hWndChild, hNode);
            return FALSE;
        }
        return TRUE;
    }

    static int __stdcall BrowseCallbackProcW(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
    {
        switch (uMsg)
        {
        case BFFM_INITIALIZED:
            SendMessage(hwnd, BFFM_SETSELECTIONW, TRUE, (LPARAM)pData);
            break;
        case BFFM_SELCHANGED:
            EnumChildWindows(hwnd, BrowseCallbackProcW_enum, 0);
        }
        return 0;
    }

    TString File::SelectFolder(const TString& title, const TString& defaultPath)
    {
        static char file_response[1024];
        static wchar_t lBuff[256];
        wchar_t* lRetval;

        BROWSEINFOW bInfo;
        LPITEMIDLIST lpItem;
        HRESULT lHResult;

        const wchar_t* aTitle = std::wstring(title.begin(), title.end()).c_str();

        if (!title.empty() && !wcscmp(aTitle, L"folder_query")) 
        { 
            strcpy(file_response, "windows_wchar"); return ""; 
        }

        /*if (quoteDetectedW(aTitle)) return tinyfd_selectFolderDialogW(L"INVALID TITLE WITH QUOTES", aDefaultPath);
        if (quoteDetectedW(aDefaultPath)) return tinyfd_selectFolderDialogW(aTitle, L"INVALID DEFAULT_PATH WITH QUOTES");*/

        lHResult = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

        bInfo.hwndOwner = glfwGetWin32Window(m_WindowHandler);
        bInfo.pidlRoot = NULL;
        bInfo.pszDisplayName = lBuff;
        bInfo.lpszTitle = aTitle && wcslen(aTitle) ? aTitle : NULL;
        if (lHResult == S_OK || lHResult == S_FALSE)
        {
            bInfo.ulFlags = BIF_USENEWUI;
        }
        bInfo.lpfn = BrowseCallbackProcW;
        bInfo.lParam = (LPARAM) defaultPath.c_str();
        bInfo.iImage = -1;

        lpItem = SHBrowseForFolderW(&bInfo);
        if (!lpItem)
        {
            lRetval = NULL;
        }
        else
        {
            SHGetPathFromIDListW(lpItem, lBuff);
            lRetval = lBuff;
        }

        if (lHResult == S_OK || lHResult == S_FALSE)
        {
            CoUninitialize();
        }

        if (!lRetval)
        {
            return "";
        }

        std::wstring wide(lRetval);
        std::string str(wide.begin(), wide.end());

        return str;
    }

}