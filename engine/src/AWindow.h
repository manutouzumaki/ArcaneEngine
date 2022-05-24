#ifndef AWINDOW_H
#define AWINDOW_H

#include "AScene.h" 

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class AWindow
{
private:
    int width;
    int height;
    char *name;
    GLFWwindow *window;

    void init();
    void loop();

    AWindow() {}
    static AWindow *instance;
    static AScene *currentScene;
public:
    static AWindow *get();
    static void free();
    static void changeScene(int index);
    static AScene *getScene();
    void run();
    static void setWidth(int width);
    static void setHeight(int height);
};

#endif
