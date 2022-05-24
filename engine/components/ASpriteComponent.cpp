#include "ASpriteComponent.h"
#include "../util/ADefines.h"
#include <imgui.h>

ASpriteComponent::ASpriteComponent(ASprite *sprite)
{
    this->sprite = sprite;
    this->color = glm::vec4(1.0f);
    this->isDirty = true;
}

ASpriteComponent::ASpriteComponent(glm::vec4 color)
{
    this->sprite = new ASprite(nullptr);
    this->color = color;
    this->isDirty = true;
}

ASpriteComponent::~ASpriteComponent()
{
    if(sprite)
    {
        delete sprite;
    }
}

void ASpriteComponent::start()
{
    lastTransform = gameObject->transform;
}

void ASpriteComponent::update(float dt)
{
    if(lastTransform != gameObject->transform)
    {
        lastTransform = gameObject->transform;
        isDirty = true;
    } 
}

void ASpriteComponent::imgui()
{
    ImGui::ColorPicker4("Color Picker", (float*)&color, 0);
    isDirty = true;
}

glm::vec4 ASpriteComponent::getColor()
{
    return color;
}

ATexture *ASpriteComponent::getTexture()
{
    return sprite->getTexture();
}

glm::vec2 *ASpriteComponent::getTexCoords()
{
    return sprite->getTexCoords(); 
}

void ASpriteComponent::setColor(glm::vec4 color)
{
    if(this->color != color)
    {
        this->color = color;
        isDirty = true;
    }
}

void ASpriteComponent::setSprite(ASprite *sprite)
{
    this->sprite = sprite; 
    isDirty = true;
}
