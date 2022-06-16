#include "AFlower.h" 
#include "../src/AGameObject.h"
#include "../util/AAssetPool.h"
#include "../components/APlayerController.h"

AFlower::AFlower()
{
    rb = nullptr;
}

void AFlower::start()
{
    if(gameObject->hasComponent("ARigidBody"))
    {
        rb = (ARigidBody *)gameObject->getComponent("ARigidBody");
        AAssetPool::getSound("powerup-appears")->play();
        rb->setIsSensor();
    }
}

void AFlower::serialize(TiXmlElement *parent)
{
    TiXmlElement *root = new TiXmlElement("AFlower");
    parent->LinkEndChild(root);
    TiXmlText *type = new TiXmlText("AFlower");
    root->LinkEndChild(type);
}

void AFlower::beginCollision(AGameObject *go, b2Contact *contact, glm::vec2 normal)
{
    if(go->hasComponent("APlayerController"))
    {
        APlayerController *playerController = (APlayerController *)go->getComponent("APlayerController");
        contact->SetEnabled(false);
        playerController->powerup();
        gameObject->destroy();
    }
}
