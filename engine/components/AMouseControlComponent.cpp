#include "AMouseControlComponent.h"
#include "../src/AWindow.h"
#include "../src/AMouseListener.h"
#include "../src/AKeyListener.h"
#include "../util/ADefines.h"

#define TIMER_COUNT 0.15F;

AMouseControlComponent::AMouseControlComponent()
{
    holdingObject = nullptr;
    timer = TIMER_COUNT;
}

AMouseControlComponent::~AMouseControlComponent()
{
    holdingObject = nullptr;
}

void AMouseControlComponent::editorUpdate(float dt)
{
    timer -= dt;
    if(holdingObject && timer <= 0.0f)
    {                                           
        holdingObject->transform->position.x = (float)((int)(floorf(AMouseListener::getOrthoX() / GRID_WIDTH)) * GRID_WIDTH) + (GRID_WIDTH*0.5f);
        holdingObject->transform->position.y = (float)((int)(floorf(AMouseListener::getOrthoY() / GRID_HEIGHT)) * GRID_HEIGHT) + (GRID_HEIGHT*0.5f);
        int x = (int)AMouseListener::getScreenX();
        int y = (int)AMouseListener::getScreenY();
        if(x >= 0 && x  <= WINDOW_WIDTH && y >= 0 && y <= WINDOW_HEIGHT)
        {
            if(AMouseListener::mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT))
            {
                place();
                timer = TIMER_COUNT;
            }
            if(AKeyListener::isKeyPressed(GLFW_KEY_ESCAPE))
            {
                holdingObject->destroy();
                holdingObject = nullptr;
            }
        }
    }
}

void AMouseControlComponent::pickupObject(AGameObject *go)
{
    if(holdingObject)
    {
        holdingObject->destroy();
        holdingObject = nullptr;
    }
    holdingObject = go;
    go->setPickable(false);
    AWindow::getScene()->addGameObject(go);
}

void AMouseControlComponent::place()
{
    AGameObject *newObj = holdingObject->copy();
    AWindow::getScene()->addGameObject(newObj);
}

