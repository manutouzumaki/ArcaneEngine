#include "AFactory.h"
#include "../util/ADefines.h"
#include "../util/AString.h"
#include "../util/AAssetPool.h"
#include "../components/ASpriteComponent.h"
#include "APhysics.h"
#include "../components/AStateMachine.h"
#include "../components/APlayerController.h"

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
        TiXmlElement *gravityScaleXml = continuousCollisionXml->NextSiblingElement();
        TiXmlElement *angularVelocityXml = gravityScaleXml->NextSiblingElement();
        TiXmlElement *frictionXml = angularVelocityXml->NextSiblingElement();
        TiXmlElement *isSensorXml = frictionXml->NextSiblingElement();
        float xVel = (float)atof(velocityXml->Attribute("x"));
        float yVel = (float)atof(velocityXml->Attribute("y"));
        float angularDamping = (float)atof(angularDampingXml->Attribute("value"));
        float linearDamping = (float)atof(linearDampingXml->Attribute("value"));
        float mass = (float)atof(massXml->Attribute("value"));
        int bodyType = (int)atof(bodyTypeXml->Attribute("value"));
        int fixedRotation = (int)atof(fixedRotationXml->Attribute("value"));
        int continuousCollision = (int)atof(continuousCollisionXml->Attribute("value"));
        float gravityScale = (float)atof(gravityScaleXml->Attribute("value"));
        float angularVelocity = (float)atof(angularVelocityXml->Attribute("value"));
        float friction = (float)atof(frictionXml->Attribute("value"));
        int isSensor = (int)atof(isSensorXml->Attribute("value"));
        return new ARigidBody(glm::vec2(xVel, yVel), angularDamping, linearDamping, mass, (BodyType)bodyType, fixedRotation, continuousCollision, gravityScale, angularVelocity, friction, isSensor);
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
    else if(StringCompare(component->GetText(), "AStateMachine", StringLength("AStateMachine")))
    {
        AStateMachine *stateMachine = new AStateMachine();
        TiXmlElement *states = component->FirstChildElement();
        for(TiXmlElement *state = states->FirstChildElement();
            state != nullptr;
            state = state->NextSiblingElement())
        {
            AAnimationState *animState = new AAnimationState();
            int NameSize = StringLength(state->GetText());
            char *name = (char *)malloc(NameSize + 1);
            StringCopy(name, NameSize, state->GetText());
            name[NameSize] = '\0';
            animState->title = name;
            TiXmlElement *loopXml = state->FirstChildElement();
            bool loop = (bool)((int)atof(loopXml->Attribute("value")));
            animState->setLoop(loop); 
            TiXmlElement *frames = loopXml->NextSiblingElement();
            for(TiXmlElement *frame = frames->FirstChildElement();
                frame != nullptr;
                frame = frame->NextSiblingElement())
            {
                ASpritesheet *sprites = AAssetPool::getSpritesheet(frame->GetText());
                TiXmlElement *spriteIndexXml = frame->FirstChildElement();
                int spriteIndex = (int)atof(spriteIndexXml->Attribute("value"));
                TiXmlElement *frameTimeXml = spriteIndexXml->NextSiblingElement();
                float frameTime = (float)atof(frameTimeXml->Attribute("value"));
                animState->addFrame(sprites->getSprite(spriteIndex), frameTime);
            }
            stateMachine->addState(animState);
        }
        stateMachine->setDefaultStateTitle(stateMachine->states[0]->title); 
        return stateMachine;
    }
    else if(StringCompare(component->GetText(), "APlayerController", StringLength("APlayerController")))
    {
        APlayerController *playerController = new APlayerController();
        return playerController; 
    }
    else
    {
        Assert(!"ERROR: component not found");
    } 
    return nullptr;
}

