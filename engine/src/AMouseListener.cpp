#include "AMouseListener.h"

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
    dragging = false;
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
    delete instance;
    printf("AMouseListener deleted\n");
}

void AMouseListener::mousePosCallback(GLFWwindow *window, double xPos, double yPos)
{
    get()->lastX = get()->xPos;
    get()->lastY = get()->yPos;
    get()->xPos = xPos;
    get()->yPos = yPos;
    get()->dragging = get()->mouseButtonPressed[0] ||
                      get()->mouseButtonPressed[1] ||
                      get()->mouseButtonPressed[2];
}

void AMouseListener::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    if(action == GLFW_PRESS)
    {
        if(button < MOUSE_MAX_BUTTONS)
        {
            get()->mouseButtonPressed[button] = true;
        }
    }
    else if(action == GLFW_RELEASE)
    {
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
}

float AMouseListener::getX()
{
    return (float)get()->xPos;
}

float AMouseListener::getY()
{
    return (float)get()->yPos;
}

float AMouseListener::getDx()
{
    return (float)(get()->xPos - get()->lastX);
}

float AMouseListener::getDy()
{
    return (float)(get()->yPos - get()->lastY);
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

