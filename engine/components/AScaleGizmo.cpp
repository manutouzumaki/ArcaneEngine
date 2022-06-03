#include "AScaleGizmo.h"
#include "../src/APrefabs.h"
#include "../src/AWindow.h"
#include "../src/AMouseListener.h"

AScaleGizmo::AScaleGizmo(ASprite *sprite, APropertiesWindow * propertiesWindow)
{
    activeGameObject = nullptr;

    gizmoWidth = 16;
    gizmoHeight = 48;

    xAxisObject = APrefabs::generateSpriteObject(sprite, gizmoWidth, gizmoHeight);
    yAxisObject = APrefabs::generateSpriteObject(sprite, gizmoWidth, gizmoHeight);

    xAxisObject->transform->zIndex = 100;
    xAxisObject->setSerializable(false);
    xAxisObject->setPickable(false);

    yAxisObject->transform->zIndex = 100;
    yAxisObject->setSerializable(false);
    yAxisObject->setPickable(false);

    xAxisSprite = (ASpriteComponent *)xAxisObject->getComponent("ASpriteComponent");
    yAxisSprite = (ASpriteComponent *)yAxisObject->getComponent("ASpriteComponent");
    AWindow::getScene()->addGameObject(xAxisObject);
    AWindow::getScene()->addGameObject(yAxisObject);
    this->propertiesWindow = propertiesWindow;

}

void AScaleGizmo::start()
{
    xAxisObject->transform->rotation = 90.0f;
    yAxisObject->transform->rotation = 180.0f;
}

void AScaleGizmo::update(float dt)
{
    if(!inUse) return;
    
    if((size_t)propertiesWindow->getActiveGameObject() !=  (size_t)xAxisObject &&
       (size_t)propertiesWindow->getActiveGameObject() !=  (size_t)yAxisObject)
    {
        activeGameObject = propertiesWindow->getActiveGameObject();
        if(activeGameObject)
        {
            setActive();
        }
        else
        {
            setInactive();
            return;
        }
    }
    
    bool xAxisHot = checkXHoverState();
    bool yAxisHot = checkYHoverState();

    if((xAxisHot || xAxisActive) && AMouseListener::isDragging() && AMouseListener::mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) && !yAxisActive)
    {
        xAxisActive = true;
    }
    else if((yAxisHot || yAxisActive) && AMouseListener::isDragging() && AMouseListener::mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) && !xAxisActive)
    {
        yAxisActive = true;    
    }
    else
    {
        xAxisActive = false;
        yAxisActive = false;
    }

    if(activeGameObject)
    {
        if(xAxisActive)
        {
            activeGameObject->transform->scale.x += AMouseListener::getWorldDx();
        }
        else if(yAxisActive)
        {
            activeGameObject->transform->scale.y += AMouseListener::getWorldDy();
        }

        xAxisObject->transform->position = activeGameObject->transform->position + glm::vec2(64, -5);
        yAxisObject->transform->position = activeGameObject->transform->position + glm::vec2(16, 61);
    }
}


void AScaleGizmo::setActive()
{
    xAxisSprite->setColor(xAxisColor);
    yAxisSprite->setColor(yAxisColor);
}

void AScaleGizmo::setInactive()
{
    xAxisSprite->setColor(glm::vec4(0, 0, 0, 0));
    yAxisSprite->setColor(glm::vec4(0, 0, 0, 0));
}

bool AScaleGizmo::checkXHoverState()
{
    glm::vec2 mouseP = glm::vec2(AMouseListener::getOrthoX(), AMouseListener::getOrthoY());
    if(mouseP.x <= xAxisObject->transform->position.x &&
       mouseP.x >= xAxisObject->transform->position.x - gizmoHeight &&
       mouseP.y >= xAxisObject->transform->position.y &&
       mouseP.y <= xAxisObject->transform->position.y + gizmoWidth)
    {
        xAxisSprite->setColor(xAxisColorHover);
        return true;
    }
    xAxisSprite->setColor(xAxisColor);
    return false;
}

bool AScaleGizmo::checkYHoverState()
{
    glm::vec2 mouseP = glm::vec2(AMouseListener::getOrthoX(), AMouseListener::getOrthoY());
    if(mouseP.x <= yAxisObject->transform->position.x &&
       mouseP.x >= yAxisObject->transform->position.x - gizmoWidth &&
       mouseP.y <= yAxisObject->transform->position.y &&
       mouseP.y >= yAxisObject->transform->position.y - gizmoHeight)
    {
        yAxisSprite->setColor(yAxisColorHover);
        return true;
    }
    yAxisSprite->setColor(yAxisColor);
    return false;
}

void AScaleGizmo::setInUse()
{
    inUse = true;
}

void AScaleGizmo::setNotUse()
{
    inUse = false;
    setInactive();
}

