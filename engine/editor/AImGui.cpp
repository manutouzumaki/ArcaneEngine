#include "AImGui.h"
#include "../util/AString.h"
#include <malloc.h>
#include <stdio.h>

void ImGuiVector2(const char *lable, glm::vec2 *data, float resetValue)
{
    ImGui::PushID(lable);
    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, 80);
    ImGui::Text(lable);
    ImGui::NextColumn();

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
    glm::vec2 buttonSize = glm::vec2(lineHeight + 3.0f, lineHeight);
    float widthEach = (ImGui::CalcItemWidth() - buttonSize.x * 2.0f) / 2.0f;
    
    ImGui::PushItemWidth(widthEach);
    if(ImGui::Button("x", ImVec2(buttonSize.x, buttonSize.y)))
    {
        data->x = resetValue;
    }
    ImGui::SameLine();
    ImGui::DragFloat("##x", &data->x, 0.1f);
    ImGui::PopItemWidth();

    ImGui::SameLine();

    ImGui::PushItemWidth(widthEach);
    if(ImGui::Button("y", ImVec2(buttonSize.x, buttonSize.y)))
    {
        data->y = resetValue;
    }
    ImGui::SameLine();
    ImGui::DragFloat("##y", &data->y, 0.1f);
    ImGui::PopItemWidth();

    ImGui::SameLine();

    ImGui::NextColumn();
    ImGui::PopStyleVar();
    ImGui::Columns(1);
    ImGui::PopID();
}

void ImGuiDragFloat(const char *lable, float *data)
{
    ImGui::PushID(lable);
    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, 80);
    ImGui::Text(lable);
    ImGui::NextColumn();

    ImGui::DragFloat("##dragFloat", data, 0.1f);

    ImGui::Columns(1);
    ImGui::PopID();
}

void ImGuiDragInt(const char *lable, int *data)
{
    ImGui::PushID(lable);
    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, 80);
    ImGui::Text(lable);
    ImGui::NextColumn();

    ImGui::DragInt("##dragFloat", data, 0.1f);

    ImGui::Columns(1);
    ImGui::PopID();
}

void ImGuiInputText(const char **name)
{
    // TODO: look for BUGS on this code
    ImGui::PushID("Name");
    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, 80);
    ImGui::Text("Name");
    ImGui::NextColumn();
     
    char buffer[256];
    StringCopy(buffer, StringLength(*name), *name);
    if(ImGui::InputText("##Name", buffer, 256))
    {
        ImGui::Columns(1);
        ImGui::PopID();
        free((void *)(*name));
        int length = StringLength(buffer);
        char *src = (char *)malloc(length + 1);
        StringCopy(src, length, buffer);
        src[length] = '\0';
        *name = (const char *)src;
        return;
    }

    ImGui::Columns(1);
    ImGui::PopID();
 
}
