#ifndef AMUSHROOMAI_H
#define AMUSHROOMAI_H

#include "../src/AComponent.h"
#include "../src/APhysics.h"
#include <glm/glm.hpp>

class AMushroomAI : public AComponent
{
private:
    bool goingRight;
    ARigidBody *rb;
    glm::vec2 speed;
    float maxSpeed;
    bool hitPlayer;
public:
    AMushroomAI();
    void start() override;
    void update(float dt) override; 
    void serialize(TiXmlElement *parent) override;
    void preSolve(AGameObject *go, b2Contact *contact, glm::vec2 normal) override;
};

#endif
