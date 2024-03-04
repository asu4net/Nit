#pragma once

namespace Nit
{
    class Window;

    class File
    {
        static void Start(const UniquePtr<Window>& window);

        String OpenFile(const String& filter);
        String SaveFile(const String& filter);

    private:
        static bool m_bInitialized;
    };
}