#ifndef AQUESTIONBLOCK_H
#define AQUESTIONBLOCK_H

#include "ABlockComponent.h"

enum BlockType
{
    COIN,
    POWERUP
};

class AQuestionBlock : public ABlockComponent
{
public:
    AQuestionBlock();
    AQuestionBlock(BlockType type);
    BlockType blockType;
    void serialize(TiXmlElement *parent) override;
    void playerHit(APlayerController *playerController) override;
    void imgui() override;
    void doCoint(APlayerController *playerController);
    void doPowerup(APlayerController *playerController);
    void spawnMushroom();
    void spawnFlower();
};

#endif
