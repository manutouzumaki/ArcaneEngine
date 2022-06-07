#ifndef AKEYLISTENER_H
#define AKEYLISTENER_H

#include <GLFW/glfw3.h>

#define KEYBOARD_MAX_BUTTONS 350

class AKeyListener
{
private:
    AKeyListener();
    static AKeyListener *instance;
    bool keyPressed[KEYBOARD_MAX_BUTTONS];
    bool keyBeginPress[KEYBOARD_MAX_BUTTONS];
public:
    static AKeyListener *get();
    static void free();
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static bool isKeyPressed(int key);
    static bool isKeyBeginPress(int key);
};

#endif
