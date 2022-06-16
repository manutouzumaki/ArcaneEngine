#include "ABlockComponent.h"
#include "../src/AGameObject.h"

ABlockComponent::ABlockComponent()
{
    bopGoingUp = true;
    doBopAnimation = false;
    bopStart = glm::vec2();
    topBopLocation = glm::vec2();
    active = true;
    bopSpeed = 0.4f;
}

void ABlockComponent::start()
{
    bopStart = gameObject->transform->position;
    topBopLocation = bopStart + glm::vec2(0.0f, 0.02f);
}

void ABlockComponent::update(float dt)
{
    if(doBopAnimation)
    {
        if(bopGoingUp)
        {
            if(gameObject->transform->position.y < topBopLocation.y)
            {
                gameObject->transform->position.y += bopSpeed * dt;
            }
            else
            {
                bopGoingUp = false;
            }
        }
        else
        {
            if(gameObject->transform->position.y > bopStart.y)
            {
                gameObject->transform->position.y -= bopSpeed * dt;
            }
            else
            { 
                gameObject->transform->position.y = bopStart.y;
                bopGoingUp = true;
                doBopAnimation = false;
            }
        }
    }
}

void ABlockComponent::beginCollision(AGameObject *go, b2Contact *contact, glm::vec2 normal)
{
    if(go->hasComponent("APlayerController"))
    {
        APlayerController *playerController = (APlayerController *)go->getComponent("APlayerController");
        if(active && normal.y < -0.8f)
        {
            doBopAnimation = true;
            playerHit(playerController);
        }
    }
}

void ABlockComponent::setInactive()
{
    active = false;
}

