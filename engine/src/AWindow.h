#ifndef AWINDOW_H
#define AWINDOW_H

#include "AScene.h" 
#include <GLFW/glfw3.h>
#include "../renderer/AFramebuffer.h"
#include "../editor/AMousePicking.h"
#include "../editor/APropertiesWindow.h"
#include "AEventSystem.h"

class AWindow : public AObserver
{
private:
    int width;
    int height;
    char *name;
    GLFWwindow *window;

    void init();
    void loop();

    AWindow();
    static AWindow *instance;
    static AScene *currentScene;

    AFramebuffer *framebuffer;
    AMousePicking *mousePicking;
    APropertiesWindow *propertiesWindow;
public:
    static AWindow *get();
    static void free();
    static void changeScene(int index);
    static AScene *getScene();
    void run();
    static int getWidth();
    static int getHeight();
    static void setWidth(int width);
    static void setHeight(int height);
    static AFramebuffer *getFramebuffer();
    static float getTargetAspectRatio();
    static AMousePicking *getMousePicking();
    static APropertiesWindow *getPropertiesWindow();

    void onNotify(AGameObject *obj, AEvent *event) override;
};

#endif
