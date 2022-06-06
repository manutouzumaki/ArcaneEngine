#include "AEditorCameraComponent.h"
#include "../src/ACamera.h"
#include "../src/AMouseListener.h"
#include "../src/AKeyListener.h"
#include "../util/ADefines.h"

#include <glm/gtx/compatibility.hpp>

AEditorCameraComponent::AEditorCameraComponent(ACamera *camera)
{
    this->camera = camera;
    clickOrigin = glm::vec3(0.0f);
    clickOriginSet = false;
    reset = false;
    lerpTime = 0.0f;
}

void AEditorCameraComponent::editorUpdate(float dt)
{
    int x = (int)AMouseListener::getScreenX();
    int y = (int)AMouseListener::getScreenY();
    if(x >= 0 && x  <= WINDOW_WIDTH && y >= 0 && y <= WINDOW_HEIGHT)
    {
        if(AMouseListener::mouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE) && !clickOriginSet)
        {
            clickOrigin = glm::vec3(AMouseListener::getOrthoX(), AMouseListener::getOrthoY(), 0.0f);
            clickOriginSet = true; 
        }

        if(AMouseListener::getScrollY() != 0.0f)
        {
            float addValue = pow(fabs(AMouseListener::getScrollY() * 0.05f), 1/camera->getZoom());
            addValue *= -fabs(AMouseListener::getScrollY()) / AMouseListener::getScrollY();
            camera->addZoom(addValue);
        }
    }

    if(clickOriginSet)
    {
        glm::vec3 mouseP = glm::vec3(AMouseListener::getOrthoX(), AMouseListener::getOrthoY(), 0.0f);
        camera->position = camera->position - (mouseP - clickOrigin);
    }

    if(!AMouseListener::mouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE))
    {
        clickOriginSet = false;
    }

    if(AKeyListener::isKeyPressed(GLFW_KEY_Q))
    {
        reset = true;
    }

    if(reset)
    {
        camera->position = glm::lerp(camera->position, glm::vec3(0.0f), lerpTime);
        camera->setZoom(camera->getZoom() + (1.0f - camera->getZoom()) * lerpTime);
        lerpTime += dt * 0.1f;
        if(fabs(camera->position.x) <= 5.0f &&
           fabs(camera->position.x) <= 5.0f)
        {
            lerpTime = 0.0f;
            camera->position = glm::vec3(0.0f);
            camera->setZoom(1.0f);
            reset = false;
        }
    }
}
