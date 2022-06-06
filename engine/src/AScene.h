#ifndef ASCENE_H
#define ASCENE_H

#include "ACamera.h"
#include "AGameObject.h"

#include "../util/AArray.h"
#include "../renderer/ARenderer.h"

#include "ASceneInitializer.h"
#include "APhysics.h"

class AScene
{
private:
    ARenderer *renderer;
    APhysics *physics;
    ACamera *camera;
    bool isRunning;
    AArray<AGameObject *> gameObjects;
    ASceneInitializer *sceneInitializer;
public:
    AScene(ASceneInitializer *sceneInitializer);
    void init();
    void start();
    void update(float dt);
    void editorUpdate(float dt);
    void render();
    void imgui();
    void save();
    void destroy();
    ~AScene();

    void addGameObject(AGameObject *gameObject);
    AGameObject *getGameObjectByIndex(int index);
    ACamera *getCamera();
    APhysics *getPhysics();
};

#endif
