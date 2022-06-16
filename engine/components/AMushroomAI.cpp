#include "AMushroomAI.h"
#include "../src/AGameObject.h"
#include "../util/AAssetPool.h"
#include "../components/APlayerController.h"
#include <math.h>

AMushroomAI::AMushroomAI()
{
    goingRight = true;
    rb = nullptr;
    speed = glm::vec2(1.0f, 0.0f);
    maxSpeed = 0.8f;
    hitPlayer = false;
}

void AMushroomAI::start()
{
    if(gameObject->hasComponent("ARigidBody"))
    {
        rb = (ARigidBody *)gameObject->getComponent("ARigidBody");
        AAssetPool::getSound("powerup-appears")->play();

    }
}

void AMushroomAI::update(float dt)
{
    if(goingRight && fabsf(rb->getVelocity().x) < maxSpeed)
    {
        rb->addVelocity(speed);
    }
    else if(!goingRight && fabsf(rb->getVelocity().x) < maxSpeed)
    {
        rb->addVelocity(glm::vec2(-speed.x, speed.y));
    }
}

void AMushroomAI::preSolve(AGameObject *go, b2Contact *contact, glm::vec2 normal)
{
    if(go->hasComponent("APlayerController"))
    {
        APlayerController *playerController = (APlayerController *)go->getComponent("APlayerController");
        contact->SetEnabled(false);
        if(!hitPlayer)
        {
            if(playerController->playerState == SMALL)
            {
                playerController->powerup();
            }
            gameObject->destroy();
            hitPlayer = true;
        }
    }
    if(fabsf(normal.y) < 0.1f)
    {
        goingRight = (normal.x < 0.0f);
    }
}

void AMushroomAI::serialize(TiXmlElement *parent)
{
    TiXmlElement *root = new TiXmlElement("AMushroomAI");
    parent->LinkEndChild(root);
    TiXmlText *type = new TiXmlText("AMushroomAI");
    root->LinkEndChild(type);
}
