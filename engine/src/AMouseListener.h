#ifndef AMOUSELISTENER_H
#define AMOUSELISTENER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#define MOUSE_MAX_BUTTONS 3

class AMouseListener
{
private:
    AMouseListener();
    static AMouseListener *instance;
    double scrollX, scrollY;
    double xPos, yPos, lastX, lastY;
    double lastWorldX, lastWorldY;
    bool mouseButtonPressed[MOUSE_MAX_BUTTONS];
    bool dragging;
    int mouseButtonsDown;
    
    glm::vec2 gameViewportPos;
    glm::vec2 gameViewportSize; 
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
    static float getScreenX();
    static float getScreenY();
    static float getDx();
    static float getDy();
    static float getWorldDx();
    static float getWorldDy();
    static float getScrollX();
    static float getScrollY();
    static bool isDragging();
    static bool mouseButtonDown(int button);
    static void setGameViewportPos(glm::vec2 gameViewportPos);
    static void setGameViewportSize(glm::vec2 gameViewportSize);
    static glm::vec2 screenToOrtho(glm::vec2 screenCoord);
    static glm::vec2 orthoToScreen(glm::vec2 worldCoord);
};

#endif
