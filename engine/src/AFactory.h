#ifndef AFACTORY_H
#define AFACTORY_H

#include "AGameObject.h"
#include <tinyxml.h>

class AFactory
{
public:
    static AGameObject *CreateGameObject(TiXmlElement *gameObject);
private:
    static AComponent *CreateComponent(TiXmlElement *component);
};

#endif
