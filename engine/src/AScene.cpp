#include "AScene.h"

AScene::AScene()
{
    camera = nullptr;
    isRunning = false;
}

void AScene::init()
{

}

void AScene::start()
{
    for(int i = 0; i < gameObjects.size(); ++i)
    {
        gameObjects[i]->start();
        renderer.add(gameObjects[i]);
    }
    isRunning = true;
}

void AScene::addGameObject(AGameObject *gameObject)
{
    if(!isRunning)
    {
        gameObjects.add(gameObject);
    }
    else
    {
        gameObjects.add(gameObject);
        gameObject->start();
        renderer.add(gameObject);
    }
}

AGameObject *AScene::getGameObjectByIndex(int index)
{
    return gameObjects[index];
}

ACamera *AScene::getCamera()
{
    return camera;
}
