#include "../util/AAssetPool.h"
#include "ALevelEditorScene.h"
#include "AWindow.h"
#include "AFactory.h"
#include "AMouseListener.h"
#include "APrefabs.h"
#include "../util/ADefines.h"
#include "../renderer/ADebugDraw.h"
#include "../components/AGridLines.h"
#include "../components/AEditorCameraComponent.h"
#include "../components/ATranslateGizmo.h"

#include <imgui.h>
#include <stdio.h>

ALevelEditorScene::ALevelEditorScene()
{
    printf("Level Editor Loading\n"); 
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
    AAssetPool::addShader("lines", "../assets/shaders/lineVertex.glsl", "../assets/shaders/lineFragment.glsl");
    AAssetPool::addShader("picking", "../assets/shaders/pickingVertex.glsl", "../assets/shaders/pickingFragment.glsl");
    
    AAssetPool::addTexture("heroTexture", "../assets/textures/poolGuy.png");
    AAssetPool::addTexture("blocksTexture", "../assets/textures/blocks.png");
    AAssetPool::addTexture("characterTexture", "../assets/textures/character.png");
    AAssetPool::addTexture("gizmoTexture", "../assets/textures/gizmos.png");

    AAssetPool::addSpritesheet("characterSpritesheet",
                               new ASpritesheet("characterSpritesheet", AAssetPool::getTexture("characterTexture"), 77, 77, 32, 0));
    AAssetPool::addSpritesheet("blocksSpritesheet",
                               new ASpritesheet("blocksSpritesheet", AAssetPool::getTexture("blocksTexture"), 16, 16, 81, 0));
    AAssetPool::addSpritesheet("gizmoSpritesheet",
                               new ASpritesheet("gizmoSpritesheet", AAssetPool::getTexture("gizmoTexture"), 24, 48, 2, 0));
}

void ALevelEditorScene::init()
{
    loadResources();
    
    ASpritesheet *gizmoSpritesheet = AAssetPool::getSpritesheet("gizmoSpritesheet");
    
    camera = new ACamera(glm::vec3(0, 0, 20));
    
    levelEditor = new AGameObject("LevelEditor");
    levelEditor->addComponent("AMouseControlComponent", new AMouseControlComponent());
    levelEditor->addComponent("AGridLines", new AGridLines());
    levelEditor->addComponent("AEditorCameraComponent", new AEditorCameraComponent(camera));
    levelEditor->addComponent("ATranslateGizmo", new ATranslateGizmo(gizmoSpritesheet->getSprite(1),
                                                                     AWindow::getPropertiesWindow())); 
    levelEditor->start();
    
    ADebugDraw::init();

    sprites[0] = AAssetPool::getSpritesheet("blocksSpritesheet");
    sprites[1] = AAssetPool::getSpritesheet("characterSpritesheet");    

    TiXmlDocument doc( "../assets/saves/tinyXmlTest.xml");
    doc.LoadFile();

    TiXmlElement *gameObjectsNode = doc.FirstChildElement();
    for(TiXmlElement *gameObject = gameObjectsNode->FirstChildElement();
        gameObject != nullptr;
        gameObject = gameObject->NextSiblingElement())
    {
        AGameObject *obj = AFactory::CreateGameObject(gameObject);
        addGameObject(obj); 
    }
}

void ALevelEditorScene::update(float dt)
{
    levelEditor->update(dt); 

    static float angle = 0.0f;
    ADebugDraw::addBox(glm::vec2(128 - 16, (128 * 2) - 16),
                       glm::vec2(32, 64),
                       glm::radians(angle),
                       glm::vec3(1, 0,0),
                       1);

    ADebugDraw::addCircle(glm::vec2(128 * 2, 128 * 2),
                          64,
                          glm::vec3(0, 0, 1),
                          1);
    angle += dt * 40;

    for(int i = 0; i < gameObjects.size(); ++i)
    {
        gameObjects[i]->update(dt);
    }
}

void ALevelEditorScene::render()
{
    renderer.render();
}

void ALevelEditorScene::close()
{
    printf("saving Scene\n");
    TiXmlDocument doc;
    TiXmlDeclaration *decl = new TiXmlDeclaration("1.0f", "", "");
    doc.LinkEndChild(decl);
    TiXmlElement *root = new TiXmlElement("GameObjects");
    doc.LinkEndChild(root);
    for(int i = 0; i < gameObjects.size(); ++i)
    {
        gameObjects[i]->serialize(root);
    }
    doc.SaveFile("../assets/saves/tinyXmlTest.xml"); 
}

void ALevelEditorScene::imgui()
{
    ImGui::Begin("Spritesheet");
    int itemSpacing = 1;
    float windowX2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
    for (int i = 0; i < sprites[0]->size(); ++i)
    {
        ASprite *sprite = sprites[0]->getSprite(i);
        size_t id = sprite->getTexID();
        ImVec2 size = ImVec2((float)sprite->getWidth() * 2, (float)sprite->getHeight() * 2);
        glm::vec2 *texCoords = sprite->getTexCoords();
        ImVec2 uv0 = ImVec2(texCoords[2].x, texCoords[0].y);
        ImVec2 uv1 = ImVec2(texCoords[0].x, texCoords[2].y);
        ImVec4 bgColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);         
        ImVec4 tintColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); 
        
        ImGui::PushID(i);
        if(ImGui::ImageButton((ImTextureID)id, size, uv0, uv1, itemSpacing, bgColor, tintColor))
        {
            AGameObject *obj = APrefabs::generateSpriteObject(sprite, sprite->getWidth() * 2, sprite->getHeight() * 2);
            AMouseControlComponent *mouseControls = (AMouseControlComponent *)levelEditor->getComponent("AMouseControlComponent");
            mouseControls->pickupObject(obj);    
        }
        ImGui::PopID();

        ImVec2 lastButtonPos = ImGui::GetItemRectMax();
        float lastButtonX2 = lastButtonPos.x;
        float nextButtonX2 = lastButtonX2 + itemSpacing + size.x;
        if(i + 1 < sprites[0]->size() && nextButtonX2 < windowX2)
        {
            ImGui::SameLine();
        }
    }
    ImGui::End();
}
