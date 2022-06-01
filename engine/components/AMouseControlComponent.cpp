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

void AMouseControlComponent::update(float dt)
{
    if(holdingObject)
    {                                           
        holdingObject->transform.position.x = (float)((int)(floorf(AMouseListener::getOrthoX() / 32)) * 32);
        holdingObject->transform.position.y = (float)((int)(floorf(AMouseListener::getOrthoY() / 32)) * 32);
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
