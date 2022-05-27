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
    this->sprite = new ASprite(nullptr, 0);
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

void ASpriteComponent::serialize(TiXmlElement *parent)
{
    TiXmlElement *root = new TiXmlElement("ASpriteComponent");
    parent->LinkEndChild(root);
    TiXmlText *type = new TiXmlText("ASpriteComponent");
    root->LinkEndChild(type);
    TiXmlElement *color = new TiXmlElement("Color");
    color->SetDoubleAttribute("r", this->color.r);
    color->SetDoubleAttribute("g", this->color.g);
    color->SetDoubleAttribute("b", this->color.b);
    color->SetDoubleAttribute("a", this->color.a);
    root->LinkEndChild(color);  
    if(this->sprite->getTexture())
    {
        TiXmlElement *texture = new TiXmlElement("Sprite");
        texture->SetAttribute("index", this->sprite->getIndex());
        TiXmlText *name = new TiXmlText(this->sprite->getTileSheet());
        texture->LinkEndChild(name);
        root->LinkEndChild(texture);
    }
    else
    {
        TiXmlElement *texture = new TiXmlElement("Sprite");
        TiXmlText *notFound = new TiXmlText("NOT_FOUND");
        texture->LinkEndChild(notFound);
        root->LinkEndChild(texture);
    }
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

ASprite *ASpriteComponent::getSprite()
{
    return this->sprite;
}



