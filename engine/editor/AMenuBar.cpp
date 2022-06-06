#include "AMenuBar.h"
#include "../src/AEventSystem.h"
#include <imgui.h>

void AMenuBar::imgui()
{
    ImGui::BeginMenuBar();
    if(ImGui::BeginMenu("File"))
    {   
        if(ImGui::MenuItem("Save", ""))
        {
            AEventSystem::notify(nullptr, new AEvent(SAVE_LEVEL)); 
        }
        if(ImGui::MenuItem("Load", ""))
        {
            AEventSystem::notify(nullptr, new AEvent(LOAD_LEVEL)); 
        }
        ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
}
