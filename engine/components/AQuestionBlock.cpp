#include "AQuestionBlock.h"
#include "../src/AWindow.h"
#include "../src/AGameObject.h"
#include "../src/APrefabs.h"
#include "AStateMachine.h"
#include "../editor/AImGui.h"

AQuestionBlock::AQuestionBlock()
    : ABlockComponent()
{
    blockType = COIN;
}

AQuestionBlock::AQuestionBlock(BlockType type)
    : ABlockComponent()
{
    blockType = type;
}

void AQuestionBlock::serialize(TiXmlElement *parent)
{
    TiXmlElement *root = new TiXmlElement("AQuestionBlock");
    parent->LinkEndChild(root);
    TiXmlText *type = new TiXmlText("AQuestionBlock");
    root->LinkEndChild(type);
    TiXmlElement *blockType = new TiXmlElement("BlockType");
    blockType->SetAttribute("value", this->blockType);
    root->LinkEndChild(blockType);
}

void AQuestionBlock::playerHit(APlayerController *playerController)
{
    switch(blockType)
    {
        case COIN:
        {
            doCoint(playerController); 
        }break;
        case POWERUP:
        {
            doPowerup(playerController);
        }break;
    }
    if(gameObject->hasComponent("AStateMachine"))
    {
        AStateMachine *stateMachine = (AStateMachine *)gameObject->getComponent("AStateMachine");
        stateMachine->trigger("inactive");
        setInactive();
    }
}

void AQuestionBlock::doCoint(APlayerController *playerController)
{
    AGameObject *coin = APrefabs::generateBlockCoin();
    coin->transform->position = gameObject->transform->position;
    coin->transform->position.y += 0.25f;
    AWindow::getScene()->addGameObject(coin);
}

void AQuestionBlock::doPowerup(APlayerController *playerController)
{
    if(playerController->playerState == SMALL)
    {
        spawnMushroom();
    }
    else
    {
        spawnFlower();
    }
}

void AQuestionBlock::spawnMushroom()
{
    AGameObject *mushroom = APrefabs::generateMushroom();
    mushroom->transform->position = gameObject->transform->position;
    mushroom->transform->position.y += 0.25f;
    AWindow::getScene()->addGameObject(mushroom);
}

void AQuestionBlock::spawnFlower()
{
    AGameObject *flower = APrefabs::generateFlower();
    flower->transform->position = gameObject->transform->position;
    flower->transform->position.y += 0.25f;
    AWindow::getScene()->addGameObject(flower);
}

void AQuestionBlock::imgui()
{
    const char *enumValues[2] = 
    {
        "COIN",
        "POWERUP",
    };
    int index = (int)blockType;
    if(ImGui::Combo("blockType", &index, enumValues, 2))
    {
        blockType = (BlockType)index; 
    }
}
