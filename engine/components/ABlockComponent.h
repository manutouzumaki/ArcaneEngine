#ifndef ABLOCKCOMPONENT_H
#define ABLOCKCOMPONENT_H

#include "../src/AComponent.h"
#include "APlayerController.h"

class ABlockComponent : public AComponent
{
private:
    bool bopGoingUp;
    bool doBopAnimation;
    glm::vec2 bopStart;
    glm::vec2 topBopLocation;
    bool active;
public:
    ABlockComponent();
    float bopSpeed;
    void start() override;
    void update(float dt) override;
    void beginCollision(AGameObject *go, b2Contact *contact, glm::vec2 normal) override;
    virtual void playerHit(APlayerController *playerController) {}
    void setInactive();
};

#endif
