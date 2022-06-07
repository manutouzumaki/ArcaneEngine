#include "AMouseControlComponent.h"
#include "../src/AWindow.h"
#include "../src/AMouseListener.h"
#include "../util/ADefines.h"

AMouseControlComponent::AMouseControlComponent()
{
    holdingObject = nullptr;
}

AMouseControlComponent::~AMouseControlComponent()
{
    holdingObject = nullptr;
}

void AMouseControlComponent::editorUpdate(float dt)
{
    if(holdingObject)
    {                                           
        holdingObject->transform->position.x = (float)((int)(floorf(AMouseListener::getOrthoX() / GRID_WIDTH)) * GRID_WIDTH);
        holdingObject->transform->position.y = (float)((int)(floorf(AMouseListener::getOrthoY() / GRID_HEIGHT)) * GRID_HEIGHT);
        int x = (int)AMouseListener::getScreenX();
        int y = (int)AMouseListener::getScreenY();
        if(x >= 0 && x  <= WINDOW_WIDTH && y >= 0 && y <= WINDOW_HEIGHT)
        {
            if(AMouseListener::mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT))
            {
                place();
            }
        }
    }
}

void AMouseControlComponent::pickupObject(AGameObject *go)
{
    holdingObject = go;
    AWindow::getScene()->addGameObject(go);
}

void AMouseControlComponent::place()
{
    holdingObject = nullptr;
}
