#include "APlayerController.h"
#include "../src/AWindow.h"
#include "../src/AKeyListener.h"
#include "AStateMachine.h"

#include "../editor/AImGui.h"
#include "../renderer/ADebugDraw.h"
#include "../util/AAssetPool.h"
#include "../src/APhysics.h"


APlayerController::APlayerController()
{
    walkSpeed = 1.9f;
    jumpBoost = 1.0f;
    jumpImpulse = 3.0f;
    slowDownForce = 0.05f;
    terminalVelocity = glm::vec2(2.1f, 3.1f);
    playerState = SMALL;
    onGround = false;
    groundDebounce = 0.0f;
    groundDebounceTime = 0.1f;
    rb = nullptr;
    stateMachine = nullptr;
    bigJumBoostFactor = 1.05f;
    playerWidth = 0.25f;
    jumpTime = 0;
    acceleration = glm::vec2();
    velocity = glm::vec2();
    isDead = false;
    enemyBounce = 0;
}

APlayerController::~APlayerController()
{

}

void APlayerController::start()
{
    if(gameObject->hasComponent("ARigidBody"))
    {
        rb = (ARigidBody *)gameObject->getComponent("ARigidBody");
        rb->setGravityScale(0.0f);
    }
    if(gameObject->hasComponent("AStateMachine"))
    {
        stateMachine = (AStateMachine *)gameObject->getComponent("AStateMachine");
    }
}

void APlayerController::update(float dt)
{
    if(AKeyListener::isKeyPressed(GLFW_KEY_RIGHT) || AKeyListener::isKeyPressed(GLFW_KEY_D))
    {
        gameObject->transform->scale.x = playerWidth;
        acceleration.x = walkSpeed;
        if(velocity.x < 0)
        {
            switch(playerState)
            {
                case SMALL: stateMachine->trigger("switchDirection"); break;
                case BIG: stateMachine->trigger("bigSwitchDirection"); break;
                case FIRE: stateMachine->trigger("fireSwitchDirection"); break;
            }
            velocity.x += slowDownForce;
        } 
        else
        {
            switch(playerState)
            {
                case SMALL: stateMachine->trigger("run"); break;
                case BIG: stateMachine->trigger("bigRun"); break;
                case FIRE: stateMachine->trigger("fireRun"); break;
            }
        }
    }
    else if(AKeyListener::isKeyPressed(GLFW_KEY_LEFT) || AKeyListener::isKeyPressed(GLFW_KEY_A))
    {
        gameObject->transform->scale.x = -playerWidth;
        acceleration.x = -walkSpeed;
        if(velocity.x > 0)
        {
            switch(playerState)
            {
                case SMALL: stateMachine->trigger("switchDirection"); break;
                case BIG: stateMachine->trigger("bigSwitchDirection"); break;
                case FIRE: stateMachine->trigger("fireSwitchDirection"); break;
            }
            velocity.x -= slowDownForce;
        } 
        else
        {
            switch(playerState)
            {
                case SMALL: stateMachine->trigger("run"); break;
                case BIG: stateMachine->trigger("bigRun"); break;
                case FIRE: stateMachine->trigger("fireRun"); break;
            }
        }
    }
    else
    {
        acceleration.x = 0.0f;
        if(velocity.x > 0)
        {
            velocity.x = fmaxf(0.0f, velocity.x - slowDownForce);
        }
        else if(velocity.x < 0)
        {
            velocity.x = fminf(0.0f, velocity.x + slowDownForce); 
        }

        if(velocity.x == 0.0f)
        {
            switch(playerState)
            {
                case SMALL: stateMachine->trigger("idle"); break;
                case BIG: stateMachine->trigger("bigIdle"); break;
                case FIRE: stateMachine->trigger("fireIdle"); break;
            }
        }
    }
    
    checkOnGround();

    if(AKeyListener::isKeyPressed(GLFW_KEY_SPACE) &&
      (jumpTime > 0 || onGround || groundDebounce > 0))
    {
        if((onGround || groundDebounce > 0) && jumpTime == 0)
        {
            AAssetPool::getSound("jump-small")->play();
            jumpTime = 28;
            velocity.y = jumpImpulse;
        }
        else if(jumpTime > 0)
        {
            jumpTime--;
            velocity.y = (jumpTime/2.2f) * jumpBoost;
        }
        else
        {
            velocity.y = 0.0f;
        }
        groundDebounce = 0;
    }
    else if(!onGround)
    {
        if(jumpTime > 0)
        {
            velocity.y *= 0.35f;
            jumpTime = 0;
        }
        groundDebounce -= dt;
        acceleration.y = AWindow::getScene()->getPhysics()->getGravity().y * 0.7f;
    }
    else
    {
        velocity.y = 0;
        acceleration.y = 0;
        groundDebounce = groundDebounceTime;
    }

    velocity += acceleration * dt;
    velocity.x = fmaxf(fminf(velocity.x, terminalVelocity.x), - terminalVelocity.x);
    velocity.y = fmaxf(fminf(velocity.y, terminalVelocity.y), - terminalVelocity.y);
    rb->setVelocity(velocity);
    rb->setAngularVelocity(0.0f);

    if(!onGround)
    {
        switch(playerState)
        {
            case SMALL: stateMachine->trigger("jump"); break;
            case BIG: stateMachine->trigger("bigJump"); break;
            case FIRE: stateMachine->trigger("fireJump"); break;
        }

    }
}

void APlayerController::checkOnGround()
{
    APhysics *physics = AWindow::getScene()->getPhysics();
    float innerPlayerWidth = playerWidth * 0.6f;
    float yVal = -0.30f;//(playerState == SMALL) ? -0.14f : -0.24f;
    
    glm::vec2 raycastBeing = gameObject->transform->position;
    raycastBeing = raycastBeing - glm::vec2(innerPlayerWidth *0.5f, 0.0f);
    glm::vec2 raycastEnd = raycastBeing + glm::vec2(0.0f, yVal);
    RaycastInfo info = physics->raycast(gameObject, raycastBeing, raycastEnd);
    
    glm::vec2 raycast2Being = gameObject->transform->position;
    raycast2Being = raycast2Being + glm::vec2(innerPlayerWidth *0.5f, 0.0f);
    glm::vec2 raycast2End = raycast2Being + glm::vec2(0.0f, yVal);
    RaycastInfo info2 = physics->raycast(gameObject, raycast2Being, raycast2End);

    onGround = (info.hit && info.hitObject != nullptr) || (info2.hit && info2.hitObject != nullptr);
}


void APlayerController::beginCollision(AGameObject *go, b2Contact *contact, glm::vec2 normal)
{
    if(isDead) return;
    
    if(go->hasComponent("AGroundComponent"))
    {
        if(fabsf(normal.x) > 0.8f)
        {
            velocity.x = 0;
        }
        else if(normal.y > 0.8f)
        {
            velocity.y = 0;
            acceleration.y = 0;
            jumpTime = 0;

        }
    }
}

void APlayerController::powerup()
{
    if(playerState == SMALL)
    {
        playerState = BIG;
        AAssetPool::getSound("powerup")->play();
        jumpBoost *= bigJumBoostFactor;
        walkSpeed *= bigJumBoostFactor;
    }
    else if(playerState == BIG)
    {
        playerState = FIRE;
        AAssetPool::getSound("powerup")->play();
    }
    stateMachine->trigger("powerup");
}

void APlayerController::serialize(TiXmlElement *parent)
{
    TiXmlElement *root = new TiXmlElement("APlayerController");
    parent->LinkEndChild(root);
    TiXmlText *type = new TiXmlText("APlayerController");
    root->LinkEndChild(type);
}
