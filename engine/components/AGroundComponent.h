#ifndef AGROUNDCOMPONENT_H
#define AGROUNDCOMPONENT_H

#include "../src/AComponent.h"

class AGroundComponent : public AComponent
{
    void serialize(TiXmlElement *parent) override;
};

#endif
