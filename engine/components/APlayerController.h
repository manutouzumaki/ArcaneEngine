#ifndef APLAYERCONTROLLER_H
#define APLAYERCONTROLLER_H

#include <glm/glm.hpp>
#include "../src/AComponent.h"

class ARigidBody;
class AStateMachine;

enum PlayerState
{
    SMALL,
    BIG,
    FIRE,
    INVINCIBLE
};

class APlayerController : public AComponent
{
public:
    float walkSpeed;
    float jumpBoost;
    float jumpImpulse;
    float slowDownForce;
    glm::vec2 terminalVelocity;
    PlayerState playerState;
    APlayerController();
    ~APlayerController();

    void start() override;
    void update(float dt) override;
    void serialize(TiXmlElement *parent) override;

    void beginCollision(AGameObject *go, b2Contact *contact, glm::vec2 normal) override;
    void powerup();
    void checkOnGround();

private:
    bool onGround;
    float groundDebounce;
    float groundDebounceTime;
    ARigidBody *rb;
    AStateMachine *stateMachine;
    float bigJumBoostFactor;
    float playerWidth;
    int jumpTime;
    glm::vec2 acceleration;
    glm::vec2 velocity;
    bool isDead;
    int enemyBounce;
};

#endif
