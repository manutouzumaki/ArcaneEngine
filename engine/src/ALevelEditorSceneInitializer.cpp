#include "ALevelEditorSceneInitializer.h"
#include "../util/AAssetPool.h"
#include "AFactory.h"
#include "APrefabs.h"
#include "../util/ADefines.h"
#include "../components/AGridLines.h"
#include "../components/AEditorCameraComponent.h"
#include "../components/AGizmoManager.h"
#include "../components/AMouseControlComponent.h"
#include "../renderer/ADebugDraw.h"
#include <imgui.h>

void ALevelEditorSceneInitializer::init(AScene *scene)
{
    sprites[0] = AAssetPool::getSpritesheet("blocksSpritesheet");
    sprites[1] = AAssetPool::getSpritesheet("characterSpritesheet");
    
    ASpritesheet *gizmoSpritesheet = AAssetPool::getSpritesheet("gizmoSpritesheet");
    levelEditor = APrefabs::generateObject();
    levelEditor->addComponent("AMouseControlComponent", new AMouseControlComponent());
    levelEditor->addComponent("AGridLines", new AGridLines());
    levelEditor->addComponent("AEditorCameraComponent", new AEditorCameraComponent(scene->getCamera()));
    levelEditor->addComponent("AGizmoManager", new AGizmoManager(gizmoSpritesheet));
    scene->addGameObject(levelEditor); 
    
    TiXmlDocument doc( "../assets/saves/tinyXmlTest.xml");
    doc.LoadFile();
    TiXmlElement *gameObjectsNode = doc.FirstChildElement();
    for(TiXmlElement *gameObject = gameObjectsNode->FirstChildElement();
        gameObject != nullptr;
        gameObject = gameObject->NextSiblingElement())
    {
        AGameObject *obj = AFactory::CreateGameObject(gameObject);
        scene->addGameObject(obj); 
    }
}

void ALevelEditorSceneInitializer::imgui()
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
