#include "AFactory.h"
#include "../util/ADefines.h"
#include "../util/AString.h"
#include "../util/AAssetPool.h"
#include "../components/ASpriteComponent.h"

#include <stdlib.h>
#include <string.h>

AGameObject *AFactory::CreateGameObject(TiXmlElement *gameObject)
{  
    int NameSize = StringLength(gameObject->GetText());
    char *name = (char *)malloc(NameSize + 1);
    StringCopy(name, NameSize, gameObject->GetText());
    name[NameSize] = '\0';

    TiXmlElement *transform = gameObject->FirstChildElement(); 
    TiXmlElement *components = transform->NextSiblingElement();
    TiXmlElement *zIndexNode = components->NextSiblingElement();

    TiXmlElement *position = transform->FirstChildElement();
    TiXmlElement *scale = position->NextSiblingElement();
    float xPos = (float)atof(position->Attribute("x"));
    float yPos = (float)atof(position->Attribute("y"));
    float xSca = (float)atof(scale->Attribute("x"));
    float ySca = (float)atof(scale->Attribute("y"));
    int zIndex = (int)atof(zIndexNode->Attribute("value"));
    
    AGameObject *obj = new AGameObject((const char *)name, ATransform(glm::vec2(xPos, yPos), glm::vec2(xSca, ySca)), zIndex);
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
        float r = (float)atof(color->Attribute("r"));
        float g = (float)atof(color->Attribute("g"));
        float b = (float)atof(color->Attribute("b"));
        float a = (float)atof(color->Attribute("a"));
        glm::vec4 colorVec = glm::vec4(r, g, b, a); 
        if(StringCompare(sprite->GetText(), "NOT_FOUND", StringLength("NOT_FOUND")))
        {
            return new ASpriteComponent(colorVec);
        }
        else
        {
            ASpritesheet *sprites = AAssetPool::getSpritesheet(sprite->GetText());
            int index = (int)atof(sprite->Attribute("index"));
            ASpriteComponent *spriteCom = new ASpriteComponent(sprites->getSprite(index));
            spriteCom->setColor(colorVec);
            return spriteCom;
        }
    }
    else
    {
        Assert(!"ERROR: component not found");
    } 
    return nullptr;
}

