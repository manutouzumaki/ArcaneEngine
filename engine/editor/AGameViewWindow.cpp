#include "AGameViewWindow.h" 
#include "../src/AWindow.h"
#include "../src/AMouseListener.h"
#include "../src/AEventSystem.h"
#include <imgui.h>

static ImVec2 getLargestSizeForViewport()
{
    ImVec2 windowSize;
    windowSize = ImGui::GetContentRegionAvail();
    windowSize.x -= ImGui::GetScrollX();
    windowSize.y -= ImGui::GetScrollY();
    float aspectWidth = windowSize.x;
    float aspectHeight = aspectWidth / AWindow::getTargetAspectRatio();
    if(aspectHeight > windowSize.y)
    {
        aspectHeight = windowSize.y;
        aspectWidth = aspectHeight * AWindow::getTargetAspectRatio();
    }
    return {aspectWidth, aspectHeight};
}

static ImVec2 getCenterPosForViewport(ImVec2 aspectSize)
{
    ImVec2 windowSize;
    windowSize = ImGui::GetContentRegionAvail();
    windowSize.x -= ImGui::GetScrollX();
    windowSize.y -= ImGui::GetScrollY();
    float viewportX = (windowSize.x * 0.5f) - (aspectSize.x * 0.5f);
    float viewportY = (windowSize.y * 0.5f) - (aspectSize.y * 0.5f);
    return {viewportX + ImGui::GetCursorPosX(), viewportY + ImGui::GetCursorPosY()};
}

bool AGameViewWindow::isPlaying = false;

void AGameViewWindow::imgui()
{
    bool condition = true;
    ImGui::Begin("Game Viewport", &condition, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_MenuBar);

    ImGui::BeginMenuBar();
    if(ImGui::MenuItem("Play", "", isPlaying, !isPlaying))
    {
        isPlaying = true;
        AEventSystem::notify(nullptr, new AEvent(GAME_ENGINE_START_PLAY));
    }
    if(ImGui::MenuItem("Stop", "", !isPlaying, isPlaying))
    {
        isPlaying = false;
        AEventSystem::notify(nullptr, new AEvent(GAME_ENGINE_STOP_PLAY));   
    }
    ImGui::EndMenuBar();

    ImVec2 windowSize = getLargestSizeForViewport();
    ImVec2 windowPos = getCenterPosForViewport(windowSize);
    ImGui::SetCursorPos(windowPos);

    ImVec2 topLeft = ImGui::GetCursorScreenPos();
    topLeft.x -= ImGui::GetScrollX();
    topLeft.y -= ImGui::GetScrollY();

    AMouseListener::setGameViewportPos(glm::vec2(topLeft.x, topLeft.y));
    AMouseListener::setGameViewportSize(glm::vec2(windowSize.x, windowSize.y));

    unsigned int textureID = AWindow::getFramebuffer()->getTextureID();
    ImVec2 uvMin = ImVec2(0.0f, 1.0f);                 
    ImVec2 uvMax = ImVec2(1.0f, 0.0f);                 
    ImVec4 tintCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   
    ImVec4 borderCol = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
    unsigned long long id = (unsigned long long)textureID; 
    ImGui::Image((ImTextureID)id, windowSize, uvMin, uvMax, tintCol, borderCol);
    ImGui::End();
}
