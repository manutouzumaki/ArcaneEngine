#ifndef ACOMPONENT_H
#define ACOMPONENT_H

#include "../util/AString.h"
#include <tinyxml.h>

#include <box2d/box2d.h>
#include <glm/glm.hpp>

class AGameObject;

class AComponent
{
public:
    ~AComponent() {}
    virtual void start() {}
    virtual void update(float dt) {} 
    virtual void editorUpdate(float dt) {} 
    virtual void destroy() {}
    virtual void imgui() {}
    virtual void serialize(TiXmlElement *parent) {}
    virtual void beginCollision(AGameObject *go, b2Contact *contact, glm::vec2 normal) {}
    virtual void endCollision(AGameObject *go, b2Contact *contact, glm::vec2 normal) {}
    virtual void preSolve(AGameObject *go, b2Contact *contact, glm::vec2 normal) {}
    virtual void postSolve(AGameObject *go, b2Contact *contact, glm::vec2 normal) {}
    AGameObject *gameObject;
    AString name;
};

#endif
