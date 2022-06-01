#ifndef ASCENE_H
#define ASCENE_H

#include "ACamera.h"
#include "AGameObject.h"

#include "../util/AArray.h"
#include "../renderer/ARenderer.h"

class AScene
{
protected:
    ARenderer renderer;
    ACamera *camera;
    bool isRunning;
    AArray<AGameObject *> gameObjects;
public:
    AScene();
    virtual void init();
    virtual void start();
    virtual void update(float dt) = 0;
    virtual void render() = 0;
    virtual void close() {};
    virtual ~AScene() {}

    void addGameObject(AGameObject *gameObject);
    AGameObject *getGameObjectByIndex(int index);
    ACamera *getCamera();
    virtual void imgui() {}
};

#endif
