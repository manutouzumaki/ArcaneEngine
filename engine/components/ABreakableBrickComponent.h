#ifndef ABREAKABLEBRICKCOMPONENT_H
#define ABREAKABLEBRICKCOMPONENT_H

#include "ABlockComponent.h"

class ABreakableBrickComponent : public ABlockComponent
{
    void serialize(TiXmlElement *parent) override;
    void playerHit(APlayerController *playerController) override;
};
#endif
