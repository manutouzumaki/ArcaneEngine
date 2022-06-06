#ifndef ACOMPONENT_H
#define ACOMPONENT_H

#include "../util/AString.h"
#include <tinyxml.h>

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
    AGameObject *gameObject;
    AString name;
};

#endif
