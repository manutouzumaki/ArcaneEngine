#ifndef ACOMPONENT_H
#define ACOMPONENT_H

#include "../util/AString.h"

class AGameObject;

class AComponent
{
public:
    virtual void start() {}
    virtual void update(float dt) = 0; 
    AGameObject *gameObject;
    AString name;
};

#endif
