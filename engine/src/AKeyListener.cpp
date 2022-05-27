#include "AKeyListener.h"

#include <stdio.h>
AKeyListener *AKeyListener::instance = nullptr;

AKeyListener::AKeyListener()
{
    for(int i = 0; i < KEYBOARD_MAX_BUTTONS; ++i)
    {
        keyPressed[i] = false;
    }
}

AKeyListener *AKeyListener::get()
{
    if(instance == nullptr)
    {
        instance = new AKeyListener();
    }
    return instance;
}

void AKeyListener::free()
{
    if(instance) delete instance;
    printf("AKeyListener deleted\n");
}

void AKeyListener::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS)
    {
        get()->keyPressed[key] = true;
    }
    else if(action == GLFW_RELEASE)
    {
        get()->keyPressed[key] = false;
    }
}

bool AKeyListener::isKeyPressed(int key)
{
    return get()->keyPressed[key];
}
