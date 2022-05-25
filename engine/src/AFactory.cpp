#include "AFactory.h"
#include "../util/ADefines.h"
#include "../util/AString.h"
#include "../util/AAssetPool.h"
#include "../components/ASpriteComponent.h"

#include <stdlib.h>

AGameObject *AFactory::CreateGameObject()
{
    TiXmlDocument doc( "../assets/saves/tinyXmlTest.xml");
    doc.LoadFile();

    const char *name = doc.FirstChildElement()->GetText();
    TiXmlElement *transform = doc.FirstChildElement()->FirstChildElement(); 
    TiXmlElement *components = transform->NextSiblingElement();
    TiXmlElement *zIndexNode = components->NextSiblingElement();

    TiXmlElement *position = transform->FirstChildElement();
    TiXmlElement *scale = position->NextSiblingElement();
    float xPos = (float)atof(position->Attribute("x"));
    float yPos = (float)atof(position->Attribute("y"));
    float xSca = (float)atof(scale->Attribute("x"));
    float ySca = (float)atof(scale->Attribute("y"));
    int zIndex = (int)atof(zIndexNode->Attribute("value"));
    
    AGameObject *obj = new AGameObject(name, ATransform(glm::vec2(xPos, yPos), glm::vec2(xSca, ySca)), zIndex);
    for(TiXmlElement *component = components->FirstChildElement();
        component != nullptr;
        component = component->NextSiblingElement())
    {
        obj->addComponent(component->GetText(), CreateComponent(component));
    }
    return obj;
}

AComponent *AFactory::CreateComponent(TiXmlElement *component)
{ 
    if(StringCompare(component->GetText(), "ASpriteComponent", StringLength("ASpriteComponent")))
    {
        TiXmlElement *color = component->FirstChildElement();
        TiXmlElement *sprite = color->NextSiblingElement();
        
        if(StringCompare(sprite->GetText(), "NOT_FOUND", StringLength("NOT_FOUND")))
        {
            float r = (float)atof(color->Attribute("r"));
            float g = (float)atof(color->Attribute("g"));
            float b = (float)atof(color->Attribute("b"));
            float a = (float)atof(color->Attribute("a"));
            return new ASpriteComponent(glm::vec4(r, g, b, a));
        }
        else
        {
            ASpritesheet *sprites = AAssetPool::getSpritesheet(sprite->GetText());
            int index = (int)atof(sprite->Attribute("index"));
            return new ASpriteComponent(sprites->getSprite(index));
        }
    }
    else
    {
        Assert(!"ERROR: component not found");
    } 
    return nullptr;
}

