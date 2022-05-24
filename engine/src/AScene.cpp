#include "AScene.h"
#include <imgui.h>

AScene::AScene()
{
    camera = nullptr;
    activeGameObject = nullptr;
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

ACamera *AScene::getCamera()
{
    return camera;
}

void AScene::sceneImgui()
{
    if(activeGameObject)
    {
        ImGui::Begin("Inspector");
        activeGameObject->imgui();
        ImGui::End();
    }
    imgui();
}
