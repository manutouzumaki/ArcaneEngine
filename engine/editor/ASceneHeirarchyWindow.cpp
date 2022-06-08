#include "ASceneHeirarchyWindow.h"
#include "../src/AWindow.h"
#include "../util/ADefines.h"
#include <imgui.h>

void ASceneHeirarchyWindow::imgui()
{
    ImGui::Begin("SceneHeirarchy");

    AArray<AGameObject *> *gameObjects = AWindow::getScene()->getGameObjects();
    int index = 0;
    for(int i = 0; i < gameObjects->size(); ++i)
    {
        if(!(*gameObjects)[i]->getSerializable() || (*gameObjects)[i]->getIsDead())
        {
            continue; 
        }

        ImGui::PushID(index);
        bool result = ImGui::TreeNodeEx((*gameObjects)[i]->name, 
                                        ImGuiTreeNodeFlags_DefaultOpen |
                                        ImGuiTreeNodeFlags_FramePadding |
                                        ImGuiTreeNodeFlags_OpenOnArrow |
                                        ImGuiTreeNodeFlags_SpanAvailWidth,
                                        (*gameObjects)[i]->name);
        ImGui::PopID();

        // start Drag and Drop
        if(ImGui::BeginDragDropSource())
        {
            size_t objectAddress = (size_t)(*gameObjects)[i];
            ImGui::SetDragDropPayload("SceneHeirarchy", &objectAddress, sizeof(size_t));
            ImGui::Text((*gameObjects)[i]->name);
            ImGui::EndDragDropSource();
        }
        if(ImGui::BeginDragDropTarget())
        {
            const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SceneHeirarchy");
            if(payload)
            {
                Assert(payload->DataSize == sizeof(size_t));
                size_t ObjectAddress = *(size_t *)payload->Data;
                AGameObject *gameObject = (AGameObject *)ObjectAddress;
                if(gameObject)
                {

                }
            }
            ImGui::EndDragDropTarget();
        }

        if(result)
        {
            ImGui::TreePop();
        }
        index++;
    }

    ImGui::End();
}

