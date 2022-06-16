#ifndef AFLOWER_H
#define AFLOWER_H

#include "../src/AComponent.h"
#include "../src/APhysics.h"

class AFlower : public AComponent
{
private:
    ARigidBody *rb;
public:
    AFlower();
    void start() override;
    void serialize(TiXmlElement *parent) override;
    void beginCollision(AGameObject *go, b2Contact *contact, glm::vec2 normal) override;
};

#endif
