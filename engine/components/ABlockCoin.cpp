#include "ABlockCoin.h"
#include "../src/AGameObject.h"
#include "../util/AAssetPool.h"
#include <math.h>


ABlockCoin::ABlockCoin()
{
    topY = glm::vec2();
    cointSpeed = 1.4f;
}

void ABlockCoin::start()
{
    topY = gameObject->transform->position + glm::vec2(0.0f, 0.5f);
    AAssetPool::getSound("coin")->play();
}

void ABlockCoin::update(float dt)
{
    if(gameObject->transform->position.y < topY.y)
    {
        gameObject->transform->position.y += cointSpeed * dt;
        gameObject->transform->scale.x -= fmodf((0.5f*dt), -1.0f);
    }
    else
    {
        gameObject->destroy();
    }
}

void ABlockCoin::serialize(TiXmlElement *parent)
{
    TiXmlElement *root = new TiXmlElement("ABlockCoin");
    parent->LinkEndChild(root);
    TiXmlText *type = new TiXmlText("ABlockCoin");
    root->LinkEndChild(type);
}

