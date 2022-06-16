#include "ABreakableBrickComponent.h"
#include "../util/AAssetPool.h"
#include "../src/AGameObject.h"

void ABreakableBrickComponent::serialize(TiXmlElement *parent)
{
    TiXmlElement *root = new TiXmlElement("ABreakableBrickComponent");
    parent->LinkEndChild(root);
    TiXmlText *type = new TiXmlText("ABreakableBrickComponent");
    root->LinkEndChild(type);
}

void ABreakableBrickComponent::playerHit(APlayerController *playerController)
{
    if(playerController->playerState == SMALL)
    {

        AAssetPool::getSound("bump")->play();
    }
    else
    {
        AAssetPool::getSound("breakBlock")->play();
        gameObject->destroy();
    }

}
