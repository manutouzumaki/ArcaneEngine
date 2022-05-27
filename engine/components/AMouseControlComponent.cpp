#include "AMouseControlComponent.h"
#include "../src/AWindow.h"
#include "../src/AMouseListener.h"

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
        holdingObject->transform.position.x = (float)(((int)AMouseListener::getOrthoX() / 32) * 32);
        holdingObject->transform.position.y = (float)(((int)AMouseListener::getOrthoY() / 32) * 32);
        if(AMouseListener::mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT))
        {
            place();
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
