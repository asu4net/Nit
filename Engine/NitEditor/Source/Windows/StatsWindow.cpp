#include "StatsWindow.h"
#include <imgui.h>

namespace Nit::StatsWindow
{
    void Show(bool* bShow)
    {
        ImGuiWindowFlags windowFlags = /*ImGuiWindowFlags_NoDecoration |*/ ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoFocusOnAppearing /*| ImGuiWindowFlags_NoNav*/;

#ifdef NIT_STATS_LOCATION_ENABLED
        ImGuiIO& io = ImGui::GetIO();
        static int location = 0;
        if (location >= 0)
        {
            const float PAD = 10.0f;
            //const ImGuiViewport* viewport = ImGui::GetMainViewport();
            //ImVec2 workPos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
            //ImVec2 workSize = viewport->WorkSize;
            ImVec2 workPos = ImGui::GetWindowPos(); // Use work area to avoid menu-bar/task-bar, if any!
            ImVec2 workSize = ImGui::GetWindowSize();
            ImVec2 windowPos, windowPosPivot;
            windowPos.x = (location & 1) ? (workPos.x + workSize.x - PAD) : (workPos.x + PAD);
            windowPos.y = (location & 2) ? (workPos.y + workSize.y - PAD) : (workPos.y + PAD);
            windowPosPivot.x = (location & 1) ? 1.0f : 0.0f;
            windowPosPivot.y = (location & 2) ? 1.0f : 0.0f;
            ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always, windowPosPivot);
            ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
            windowFlags |= ImGuiWindowFlags_NoMove;
        }
        else if (location == -2)
        {
            // Center window
            ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
            windowFlags |= ImGuiWindowFlags_NoMove;
        }
#endif
        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
        if (ImGui::Begin("Stats", bShow, windowFlags))
        {
            String statsText;
            statsText.append("MousePos: " + Input::GetMousePosition().ToString());
            statsText.append("\nFPS: " + std::to_string(Time::GetFrameRate()));
            statsText.append("\nAppTime: " + std::to_string(Time::GetApplicationTime()));
            statsText.append("\nAppFrames: " + std::to_string(Time::GetApplicationFrames()));
            statsText.append("\nDeltaTime: " + std::to_string(Time::GetDeltaTime()));
            statsText.append("\nFixedUpdateCalls: " + std::to_string(Time::GetFixedUpdateCalls()));
            ImGui::Text(statsText.c_str());
#ifdef NIT_STATS_LOCATION_ENABLED
            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::MenuItem("Custom", NULL, location == -1)) location = -1;
                if (ImGui::MenuItem("Center", NULL, location == -2)) location = -2;
                if (ImGui::MenuItem("Top-left", NULL, location == 0)) location = 0;
                if (ImGui::MenuItem("Top-right", NULL, location == 1)) location = 1;
                if (ImGui::MenuItem("Bottom-left", NULL, location == 2)) location = 2;
                if (ImGui::MenuItem("Bottom-right", NULL, location == 3)) location = 3;
                if (bOpen && ImGui::MenuItem("Close")) *bOpen = false;
                ImGui::EndPopup();
            }
#endif
        }
        ImGui::End();
    }
}