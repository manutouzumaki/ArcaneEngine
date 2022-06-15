#include "AMouseListener.h"
#include "AWindow.h"

#include "../src/AWindow.h"
#include "../util/ADefines.h"
#include <stdio.h>

AMouseListener *AMouseListener::instance = nullptr;

AMouseListener::AMouseListener()
{
    scrollX = 0.0;
    scrollY = 0.0;
    xPos = 0.0;
    yPos = 0.0;
    lastX = 0.0;
    lastY = 0.0;
    lastWorldX = 0.0f;
    lastWorldY = 0.0;
    dragging = false;
    mouseButtonsDown = 0;
    for(int i = 0; i < MOUSE_MAX_BUTTONS; ++i)
    {
        mouseButtonPressed[i] = false;
    }
}

AMouseListener *AMouseListener::get()
{
    if(instance == nullptr)
    {
        instance = new AMouseListener(); 
    }
    return instance;
}

void AMouseListener::free()
{
    if(instance) delete instance;
    printf("AMouseListener deleted\n");
}

void AMouseListener::mousePosCallback(GLFWwindow *window, double xPos, double yPos)
{
    if(get()->mouseButtonsDown > 0)
    {
        get()->dragging = true;
    }
    get()->xPos = xPos;
    get()->yPos = yPos;
}

void AMouseListener::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    if(action == GLFW_PRESS)
    {
        get()->mouseButtonsDown++;
        if(button < MOUSE_MAX_BUTTONS)
        {
            get()->mouseButtonPressed[button] = true;
        }
    }
    else if(action == GLFW_RELEASE)
    {
        get()->mouseButtonsDown--;
        if(button < MOUSE_MAX_BUTTONS)
        {
            get()->mouseButtonPressed[button] = false;
            get()->dragging = false; 
        }
    }
}

void AMouseListener::mouseScrollCallback(GLFWwindow *window, double scrollX, double scrollY)
{
    get()->scrollX = scrollX;
    get()->scrollY = scrollY;
}

void AMouseListener::endFrame()
{
    get()->scrollX = 0;
    get()->scrollY = 0;
    get()->lastX = get()->xPos;
    get()->lastY = get()->yPos;
    get()->lastWorldX = get()->getOrthoX();
    get()->lastWorldY = get()->getOrthoY();
}

float AMouseListener::getX()
{
    return (float)get()->xPos;
}

float AMouseListener::getY()
{
    return (float)get()->yPos;
}

float AMouseListener::getOrthoX()
{
    float normalizeMouseX = ((float)getX() - get()->gameViewportPos.x) / get()->gameViewportSize.x;
    normalizeMouseX = normalizeMouseX * 2 - 1;
    glm::vec4 position = glm::vec4(normalizeMouseX, 0, 0, 1);
    ACamera *camera = AWindow::getScene()->getCamera();
    position = camera->getInvViewMatrix() * camera->getInvProjMatrix() * position;
    return position.x;
}


float AMouseListener::getOrthoY()
{
    float normalizeMouseY = ((float)getY() - get()->gameViewportPos.y) / get()->gameViewportSize.y;
    normalizeMouseY = normalizeMouseY * 2 - 1;
    glm::vec4 position = glm::vec4(0, -normalizeMouseY, 0, 1);
    ACamera *camera = AWindow::getScene()->getCamera();
    position = camera->getInvViewMatrix() * camera->getInvProjMatrix() * position;
    return position.y;
}

float AMouseListener::getScreenX()
{
    float normalizeMouseX = ((float)getX() - get()->gameViewportPos.x) / get()->gameViewportSize.x;
    normalizeMouseX = normalizeMouseX * (float)WINDOW_WIDTH;
    return normalizeMouseX;
}

float AMouseListener::getScreenY()
{
    float normalizeMouseY = ((float)getY() - get()->gameViewportPos.y) / get()->gameViewportSize.y;
    normalizeMouseY = (float)WINDOW_HEIGHT - (normalizeMouseY * (float)WINDOW_HEIGHT);
    return normalizeMouseY;
}

float AMouseListener::getDx()
{
    return (float)(get()->xPos - get()->lastX);
}

float AMouseListener::getDy()
{
    return (float)(get()->yPos - get()->lastY);
}

float AMouseListener::getWorldDx()
{
    return (float)(get()->getOrthoX() - get()->lastWorldX);
}

float AMouseListener::getWorldDy()
{
    return (float)(get()->getOrthoY() - get()->lastWorldY);
}


float AMouseListener::getScrollX()
{
    return (float)get()->scrollX;
}

float AMouseListener::getScrollY()
{
    return (float)get()->scrollY;
}

bool AMouseListener::isDragging()
{
    return get()->dragging;
}

bool AMouseListener::mouseButtonDown(int button)
{
    if(button < MOUSE_MAX_BUTTONS)
    {
        return get()->mouseButtonPressed[button];
    }
    return false;
}

void AMouseListener::setGameViewportPos(glm::vec2 gameViewportPos)
{
    get()->gameViewportPos = gameViewportPos;
}

void AMouseListener::setGameViewportSize(glm::vec2 gameViewportSize)
{
    get()->gameViewportSize = gameViewportSize;
}

glm::vec2 AMouseListener::screenToOrtho(glm::vec2 screenCoord)
{
    glm::vec2 normalizeP = (screenCoord - get()->gameViewportPos) / get()->gameViewportSize;
    normalizeP = (normalizeP * 2.0f) - glm::vec2(1);
    glm::vec4 position = glm::vec4(normalizeP.x, -normalizeP.y, 0, 1);
    ACamera *camera = AWindow::getScene()->getCamera();
    position = camera->getInvViewMatrix() * camera->getInvProjMatrix() * position;
    return glm::vec2(position.x, position.y);
}

glm::vec2 AMouseListener::orthoToScreen(glm::vec2 worldCoord)
{
    ACamera *camera = AWindow::getScene()->getCamera();
    glm::vec4 ndcSpacePos = glm::vec4(worldCoord.x, worldCoord.y, 0, 1);
    ndcSpacePos = camera->getProjMatrix() * camera->getViewMatrix() * ndcSpacePos;
    glm::vec2 windowSpace = glm::vec2(ndcSpacePos.x, ndcSpacePos.y) * (1.0f/ndcSpacePos.w);
    windowSpace = windowSpace + glm::vec2(0.5f);
    windowSpace = windowSpace * glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT);
    return windowSpace;
}
