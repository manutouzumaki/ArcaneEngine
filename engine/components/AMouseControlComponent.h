#ifndef AMOUSECONTROLCOMPONENT_H
#define AMOUSECONTROLCOMPONENT_H

#include "../src/AGameObject.h"

class AMouseControlComponent : public AComponent
{
public:
    AMouseControlComponent();
    ~AMouseControlComponent();
    void update(float dt) override;
    void pickupObject(AGameObject *go);
    void place();
private:
    AGameObject *holdingObject;
};

#endif
