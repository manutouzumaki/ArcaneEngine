#include "ATranslateGizmo.h"
#include "../src/APrefabs.h"
#include "../src/AWindow.h"

ATranslateGizmo::ATranslateGizmo(ASprite *sprite, APropertiesWindow * propertiesWindow)
{
    activeGameObject = nullptr;
    xAxisObject = APrefabs::generateSpriteObject(sprite, 16, 48);
    yAxisObject = APrefabs::generateSpriteObject(sprite, 16, 48);

    xAxisObject->setZIndex(100);
    xAxisObject->setSerializable(false);

    yAxisObject->setZIndex(100);
    yAxisObject->setSerializable(false);

    xAxisSprite = (ASpriteComponent *)xAxisObject->getComponent("ASpriteComponent");
    yAxisSprite = (ASpriteComponent *)yAxisObject->getComponent("ASpriteComponent");
    AWindow::getScene()->addGameObject(xAxisObject);
    AWindow::getScene()->addGameObject(yAxisObject);
    this->propertiesWindow = propertiesWindow;

}

void ATranslateGizmo::start()
{
    xAxisObject->transform.rotation = 90.0f;
    yAxisObject->transform.rotation = 180.0f;
}

void ATranslateGizmo::update(float dt)
{
    if(activeGameObject)
    {
        xAxisObject->transform.position = activeGameObject->transform.position + glm::vec2(64, -5);
        yAxisObject->transform.position = activeGameObject->transform.position + glm::vec2(16, 61);
    }
    
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
        }
    }
}


void ATranslateGizmo::setActive()
{
    xAxisSprite->setColor(xAxisColor);
    yAxisSprite->setColor(yAxisColor);
}

void ATranslateGizmo::setInactive()
{
    xAxisSprite->setColor(glm::vec4(0, 0, 0, 0));
    yAxisSprite->setColor(glm::vec4(0, 0, 0, 0));
}
