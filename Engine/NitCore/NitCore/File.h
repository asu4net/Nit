#pragma once

namespace Nit
{
    class CWindow;

    class File
    {
    public:
        static void Init(const TUniquePtr<CWindow>& window);

        static TString OpenFile(const TString& filter);
        static TString SaveFile(const TString& filter);
        static TString SelectFolder(const TString& title = "", const TString& defaultPath = "");

    private:
        static bool m_bInitialized;
    };
}