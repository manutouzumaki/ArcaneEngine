#include "AGizmoManager.h"
#include "../src/AWindow.h"
#include "../src/AKeyListener.h"

AGizmoManager::AGizmoManager(ASpritesheet *spritesheet)
{
    this->spritesheet = spritesheet;
}

void AGizmoManager::start()
{
    gameObject->addComponent("ATranslateGizmo", new ATranslateGizmo(spritesheet->getSprite(1), AWindow::getPropertiesWindow()));
    gameObject->addComponent("AScaleGizmo", new AScaleGizmo(spritesheet->getSprite(2), AWindow::getPropertiesWindow()));
    translateGizmo = (ATranslateGizmo *)gameObject->getComponent("ATranslateGizmo");
    scaleGizmo = (AScaleGizmo *)gameObject->getComponent("AScaleGizmo"); 
    gizmoInUse = 0;
}

void AGizmoManager::editorUpdate(float dt)
{
    if(gizmoInUse == 0)
    {
        translateGizmo->setInUse();
        scaleGizmo->setNotUse();
    }
    else if(gizmoInUse == 1)
    {
        translateGizmo->setNotUse();
        scaleGizmo->setInUse();
    }

    if(AKeyListener::isKeyPressed(GLFW_KEY_T))
    {
        gizmoInUse = 0;
    }
    else if(AKeyListener::isKeyPressed(GLFW_KEY_S))
    {
        gizmoInUse = 1;
    }

}
