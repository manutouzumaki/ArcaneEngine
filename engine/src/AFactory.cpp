#include "AFactory.h"
#include "../util/ADefines.h"
#include "../util/AString.h"
#include "../util/AAssetPool.h"
#include "../components/ASpriteComponent.h"
#include "APhysics.h"

#include <stdlib.h>
#include <string.h>

AGameObject *AFactory::CreateGameObject(TiXmlElement *gameObject)
{  
    int NameSize = StringLength(gameObject->GetText());
    char *name = (char *)malloc(NameSize + 1);
    StringCopy(name, NameSize, gameObject->GetText());
    name[NameSize] = '\0';

    TiXmlElement *components = gameObject->FirstChildElement(); 
 
    AGameObject *obj = new AGameObject((const char *)name, true);
    for(TiXmlElement *component = components->FirstChildElement();
        component != nullptr;
        component = component->NextSiblingElement())
    {
        int componentSize = StringLength(component->GetText());
        char *componentName = (char *)malloc(componentSize + 1);
        StringCopy(componentName, componentSize, component->GetText());
        componentName[componentSize] = '\0';

        obj->addComponent(componentName, CreateComponent(component));
        if(StringCompare(componentName, "ATransformComponent", StringLength("ATransformComponent")))
        {
            obj->transform = (ATransformComponent *)obj->getComponent("ATransformComponent");
        }
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
    else if(StringCompare(component->GetText(), "ATransformComponent", StringLength("ATransformComponent")))
    {
        TiXmlElement *position = component->FirstChildElement();
        TiXmlElement *scale = position->NextSiblingElement();
        TiXmlElement *rotation = scale->NextSiblingElement();
        TiXmlElement *zIndexNode = rotation->NextSiblingElement();
        float xPos = (float)atof(position->Attribute("x"));
        float yPos = (float)atof(position->Attribute("y"));
        float xSca = (float)atof(scale->Attribute("x"));
        float ySca = (float)atof(scale->Attribute("y"));
        float rotAngle = (float)atof(rotation->Attribute("angle"));
        int zIndex = (int)atof(zIndexNode->Attribute("value"));
        return new ATransformComponent(glm::vec2(xPos, yPos), glm::vec2(xSca, ySca), rotAngle, zIndex);
    }
    else if(StringCompare(component->GetText(), "ARigidBody", StringLength("ARigidBody")))
    {
        TiXmlElement *velocityXml = component->FirstChildElement();
        TiXmlElement *angularDampingXml = velocityXml->NextSiblingElement();
        TiXmlElement *linearDampingXml = angularDampingXml->NextSiblingElement();
        TiXmlElement *massXml = linearDampingXml->NextSiblingElement();
        TiXmlElement *bodyTypeXml = massXml->NextSiblingElement();
        TiXmlElement *fixedRotationXml = bodyTypeXml->NextSiblingElement();
        TiXmlElement *continuousCollisionXml = fixedRotationXml->NextSiblingElement();
        float xVel = (float)atof(velocityXml->Attribute("x"));
        float yVel = (float)atof(velocityXml->Attribute("y"));
        float angularDamping = (float)atof(angularDampingXml->Attribute("value"));
        float linearDamping = (float)atof(linearDampingXml->Attribute("value"));
        float mass = (float)atof(massXml->Attribute("value"));
        int bodyType = (int)atof(bodyTypeXml->Attribute("value"));
        int fixedRotation = (int)atof(fixedRotationXml->Attribute("value"));
        int continuousCollision = (int)atof(continuousCollisionXml->Attribute("value"));
        return new ARigidBody(glm::vec2(xVel, yVel), angularDamping, linearDamping, mass, (BodyType)bodyType, fixedRotation, continuousCollision);
    }
    else if(StringCompare(component->GetText(), "ABoxCollider", StringLength("ABoxCollider")))
    {
        TiXmlElement *offsetXml = component->FirstChildElement();
        TiXmlElement *halfSizeXml = offsetXml->NextSiblingElement();
        TiXmlElement *originXml = halfSizeXml->NextSiblingElement(); 
        float xOff = (float)atof(offsetXml->Attribute("x"));
        float yOff = (float)atof(offsetXml->Attribute("y"));
        float xHalf = (float)atof(halfSizeXml->Attribute("x"));
        float yHalf = (float)atof(halfSizeXml->Attribute("y"));
        float xOri = (float)atof(originXml->Attribute("x"));
        float yOri = (float)atof(originXml->Attribute("y"));
        return new ABoxCollider(glm::vec2(xOff, yOff), glm::vec2(xHalf, yHalf), glm::vec2(xOri, yOri));
    }
    else if(StringCompare(component->GetText(), "ACircleCollider", StringLength("ACircleCollider")))
    {
        TiXmlElement *offsetXml = component->FirstChildElement();
        TiXmlElement *radiusXml = offsetXml->NextSiblingElement();
        float xOff = (float)atof(offsetXml->Attribute("x"));
        float yOff = (float)atof(offsetXml->Attribute("y"));
        float radius = (float)atof(radiusXml->Attribute("value"));
        return new ACircleCollider(glm::vec2(xOff, yOff), radius);
    }
    else
    {
        Assert(!"ERROR: component not found");
    } 
    return nullptr;
}

