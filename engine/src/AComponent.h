#ifndef ACOMPONENT_H
#define ACOMPONENT_H

#include "../util/AString.h"

class AGameObject;

class AComponent
{
public:
    virtual void start() {}
    virtual void update(float dt) {}; 
    virtual void imgui() {}
    AGameObject *gameObject;
    AString name;
};

#endif
