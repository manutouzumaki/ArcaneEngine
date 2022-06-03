#include "APropertiesWindow.h"
#include "../src/AScene.h"
#include "../src/AMouseListener.h"
#include "../src/AWindow.h"
#include "../src/APhysics.h"
#include <imgui.h>

APropertiesWindow::APropertiesWindow()
{
    activeGameObject = nullptr;
    timer = 0.2f;
}

void APropertiesWindow::update(float dt, AScene *currentScene)
{
    timer -= dt;
    if(AMouseListener::mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) && timer < 0.0f && !AMouseListener::isDragging())
    {
        int x = (int)AMouseListener::getScreenX();
        int y = (int)AMouseListener::getScreenY();
        if(x >= 0 && x  <= WINDOW_WIDTH && y >= 0 && y <= WINDOW_HEIGHT)
        {
            int index = AWindow::getMousePicking()->readPixel(x, y);
            if(index >= 0)
            {
                AGameObject *obj = currentScene->getGameObjectByIndex(index);
                if(obj->getPickable())
                {
                    activeGameObject = obj;
                }
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
        ImGui::Begin("Properties");
        if(ImGui::BeginPopupContextWindow("Commponent Adder"))
        {
            if(ImGui::MenuItem("Add ARigidBody"))
            {
                if(!activeGameObject->hasComponent("ARigidBody"))
                {
                    activeGameObject->addComponent("ARigidBody", new ARigidBody());
                }
            }
            if(ImGui::MenuItem("Add ABoxCollider"))
            {
                if(!activeGameObject->hasComponent("ABoxCollider") && !activeGameObject->hasComponent("ACircleCollider"))
                {
                    activeGameObject->addComponent("ABoxCollider", new ABoxCollider());
                }
            }
            if(ImGui::MenuItem("Add ACircleCollider"))
            {
                if(!activeGameObject->hasComponent("ACircleCollider") && !activeGameObject->hasComponent("ABoxCollider"))
                {
                    activeGameObject->addComponent("ACircleCollider", new ACircleCollider());
                }
            }
            ImGui::EndPopup();
        }
        activeGameObject->imgui();
        ImGui::End();
    }
}

AGameObject *APropertiesWindow::getActiveGameObject()
{
    return activeGameObject;
}
