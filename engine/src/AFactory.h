#ifndef AFACTORY_H
#define AFACTORY_H

#include "AGameObject.h"
#include <tinyxml.h>

class AFactory
{
public:
    static AGameObject *CreateGameObject();
    static AComponent *CreateComponent(TiXmlElement *component);
};

#endif
