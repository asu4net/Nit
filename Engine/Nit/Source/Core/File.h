#pragma once

namespace Nit
{
    class Window;

    class File
    {
    public:
        static void Init(const UniquePtr<Window>& window);

        static String OpenFile(const String& filter);
        static String SaveFile(const String& filter);
        static String SelectFolder(const String& title = "", const String& defaultPath = "");

    private:
        static bool m_bInitialized;
    };
}