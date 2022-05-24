#include "ALevelEditorScene.h"
#include "../util/ADefines.h"
#include "../util/AAssetPool.h"

#include <imgui.h>
#include <stdio.h>
#include <glad/glad.h>


ALevelEditorScene::ALevelEditorScene()
{
    printf("Level Editor Scene\n");
}

ALevelEditorScene::~ALevelEditorScene()
{
    printf("closing Level Editor Scene...\n");
    delete camera;
    for(int i = 0; i < gameObjects.size(); ++i)
    {
        delete gameObjects[i];
    }
}

void loadResources()
{
    AAssetPool::addShader("default", "../assets/shaders/vertex.glsl", "../assets/shaders/fragment.glsl");
    AAssetPool::addTexture("heroTexture", "../assets/textures/poolGuy.png");
    AAssetPool::addTexture("colorsTexture", "../assets/textures/colors.png");
    AAssetPool::addTexture("characterTexture", "../assets/textures/character.png");
    AAssetPool::addSpritesheet("colorsSpritesheet", new ASpritesheet(AAssetPool::getTexture("colorsTexture"), 16, 16, 32, 0));
    AAssetPool::addSpritesheet("characterSpritesheet", new ASpritesheet(AAssetPool::getTexture("characterTexture"), 77, 77, 32, 0));
}

void ALevelEditorScene::init()
{
    camera = new ACamera(glm::vec3(0, 0, 20));
    
    loadResources();

    ASpritesheet *sprites = AAssetPool::getSpritesheet("colorsSpritesheet");
    ASpritesheet *characterSprites = AAssetPool::getSpritesheet("characterSpritesheet");
    
    float size = 20.0f;
    int width = WINDOW_WIDTH / (int)size;
    int height = WINDOW_HEIGHT / (int)size;    
    for(int x = 0; x < width; ++x)
    {
        for(int y = 0; y < height; ++y)
        {
            AGameObject *obj = new AGameObject("obj", ATransform(glm::vec2((float)x * size, (float)y * size), glm::vec2(size, size)), 1);
            obj->addComponent("ASpriteComponent", new ASpriteComponent(glm::vec4(x/(float)width, y/(float)height, 0.0f, 0.6f)));
            addGameObject(obj);
        }
    }

    hero = new AGameObject("hero", ATransform(glm::vec2(0, 0), glm::vec2(100, 100)), 0);
    hero->addComponent("ASpriteComponent", new ASpriteComponent(characterSprites->getSprite(31)));
    
    addGameObject(hero);
    activeGameObject = hero;
}

void ALevelEditorScene::update(float dt)
{ 

    printf("%d FPS\n", (int)(1.0f/dt));
 
    ASpritesheet *characterSprites = AAssetPool::getSpritesheet("characterSpritesheet");
    ASpriteComponent *sprite = (ASpriteComponent *)hero->getComponent("ASpriteComponent");
    static float counter = 16.0f;
    sprite->setSprite(characterSprites->getSprite((int)counter));
    counter += dt * 8.0f;
    if(counter >= 16.0f + 8.0f)
    {
        counter = 16.0;
    }

    for(int i = 0; i < gameObjects.size(); ++i)
    {
        gameObjects[i]->update(dt);
    }

    renderer.render();
}

void ALevelEditorScene::imgui()
{
    ImGui::Begin("Test Window");
    ImGui::Text("Hellow World");   
    ImGui::End();
}
