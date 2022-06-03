#ifndef AIMGUI_H
#define AIMGUI_H

#include <imgui.h>
#include <glm/glm.hpp>

void ImGuiVector2(const char *lable, glm::vec2 *data, float resetValue);
void ImGuiDragFloat(const char *lable, float *data);
void ImGuiDragInt(const char *lable, int *data);

#endif
