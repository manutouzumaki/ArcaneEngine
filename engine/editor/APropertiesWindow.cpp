#include "APropertiesWindow.h"
#include "../src/AScene.h"
#include "../src/AMouseListener.h"
#include "../src/AWindow.h"
#include <imgui.h>

APropertiesWindow::APropertiesWindow()
{
    activeGameObject = nullptr;
    timer = 0.2f;
}

void APropertiesWindow::update(float dt, AScene *currentScene)
{
    timer -= dt;
    if(AMouseListener::mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) && timer < 0.0f)
    {
        int x = (int)AMouseListener::getScreenX();
        int y = (int)AMouseListener::getScreenY();
        if(x >= 0 && x  <= WINDOW_WIDTH && y >= 0 && y <= WINDOW_HEIGHT)
        {
            int index = AWindow::getMousePicking()->readPixel(x, y);
            if(index >= 0)
            {
                activeGameObject = currentScene->getGameObjectByIndex(index);
            }
            else
            {
                activeGameObject = nullptr;
            }
        }
        timer = 0.2f;
    }
}

void APropertiesWindow::imgui()
{
    if(activeGameObject)
    {
        ImGui::Begin("Inspector");
        activeGameObject->imgui();
        ImGui::End();
    }
}

AGameObject *APropertiesWindow::getActiveGameObject()
{
    return activeGameObject;
}
