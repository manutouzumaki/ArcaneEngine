#ifndef APROPERTIESWINDOW_H
#define APROPERTIESWINDOW_H

class AGameObject;
class AScene;

class APropertiesWindow
{
private:
    AGameObject *activeGameObject;
    float timer;
public:
    APropertiesWindow();
    void update(float dt, AScene *currentScene);
    void imgui();
    AGameObject *getActiveGameObject();
    void setActiveGameObject(AGameObject *activeGameObject);
};

#endif
