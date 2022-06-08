#include "ATransformComponent.h"

#include "../editor/AImGui.h"
#include "../src/AGameObject.h"

ATransformComponent::ATransformComponent()
{
    init(glm::vec2(0.0f), glm::vec2(1.0f));
}

ATransformComponent::ATransformComponent(glm::vec2 position)
{
    init(position, glm::vec2(1.0f));
}

ATransformComponent::ATransformComponent(glm::vec2 position, glm::vec2 scale,
                                         float rotation, int zIndex)
{
    init(position, scale);
    this->rotation = rotation;
    this->zIndex = zIndex;
}
void ATransformComponent::init(glm::vec2 position, glm::vec2 scale)
{
    this->position = position;
    this->scale = scale;
    this->rotation = 0.0f;
    this->zIndex = 1;
}

ATransformComponent::~ATransformComponent()
{
    if(name.get())
    {
        free((void *)name.get());
    }
}

bool ATransformComponent::operator==(ATransformComponent *a)
{
    bool positionCheck = (a->position.x == position.x &&
                          a->position.y == position.y);
    bool scaleCheck = (a->scale.x == scale.x &&
                       a->scale.y == scale.y);
    bool rotationCheck = (a->rotation == rotation);
    bool zIndexCheck = a->zIndex == zIndex;
    return (positionCheck && scaleCheck && rotationCheck && zIndexCheck);
}

bool ATransformComponent::operator!=(ATransformComponent *a)
{
    bool positionCheck = (a->position.x == position.x &&
                          a->position.y == position.y);
    bool scaleCheck = (a->scale.x == scale.x &&
                       a->scale.y == scale.y);
    bool rotationCheck = (a->rotation == rotation);
    bool zIndexCheck = a->zIndex == zIndex;
    return (!positionCheck || !scaleCheck || !rotationCheck || !zIndexCheck);
}

void ATransformComponent::imgui()
{
    ImGuiInputText(&gameObject->name);
    ImGuiVector2("Position", &this->position, 0.0f);
    ImGuiVector2("Scale", &this->scale, 32.0f);
    ImGuiDragFloat("Rotation", &this->rotation);
    ImGuiDragInt("zIndex", &this->zIndex);
}

void ATransformComponent::serialize(TiXmlElement *parent)
{
    TiXmlElement *root = new TiXmlElement("ATransformComponent");
    parent->LinkEndChild(root);
    TiXmlText *type = new TiXmlText("ATransformComponent");
    root->LinkEndChild(type);
    TiXmlElement *position = new TiXmlElement("Position");
    position->SetDoubleAttribute("x", this->position.x);
    position->SetDoubleAttribute("y", this->position.y);
    root->LinkEndChild(position);
    TiXmlElement *scale = new TiXmlElement("Scale");
    scale->SetDoubleAttribute("x", this->scale.x);
    scale->SetDoubleAttribute("y", this->scale.y);
    root->LinkEndChild(scale);
    TiXmlElement *rotation = new TiXmlElement("Rotation");
    rotation->SetDoubleAttribute("angle", this->rotation);
    root->LinkEndChild(rotation);
    TiXmlElement *zIndex = new TiXmlElement("ZIndex");
    zIndex->SetAttribute("value", this->zIndex);
    root->LinkEndChild(zIndex);
}

