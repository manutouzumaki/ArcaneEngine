#ifndef AMOUSELISTENER_H
#define AMOUSELISTENER_H

#include <GLFW/glfw3.h>

#define MOUSE_MAX_BUTTONS 3

class AMouseListener
{
private:
    AMouseListener();
    static AMouseListener *instance;
    double scrollX, scrollY;
    double xPos, yPos, lastX, lastY;
    bool mouseButtonPressed[MOUSE_MAX_BUTTONS];
    bool dragging;
public:
    static AMouseListener *get();
    static void free();
    static void mousePosCallback(GLFWwindow *window, double xPos, double yPos);
    static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    static void mouseScrollCallback(GLFWwindow *window, double scrollX, double scrollY);
    static void endFrame();

    static float getX();
    static float getY();
    static float getOrthoX();
    static float getOrthoY();
    static float getDx();
    static float getDy();
    static float getScrollX();
    static float getScrollY();
    static bool isDragging();
    static bool mouseButtonDown(int button);
};

#endif
