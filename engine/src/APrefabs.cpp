#include "APrefabs.h"
#include <glm/glm.hpp>
#include "../components/ASpriteComponent.h"
#include "../components/ATransformComponent.h"
#include "../components/AStateMachine.h"
#include "../util/AAssetPool.h"
#include "../util/AString.h"

AGameObject *APrefabs::generateSpriteObject(ASprite *sprite, float sizeX, float sizeY)
{
    int NameSize = StringLength("Sprite_Object_Gen");
    char *name = (char *)malloc(NameSize + 1);
    StringCopy(name, NameSize, "Sprite_Object_Gen");
    name[NameSize] = '\0';

    AGameObject *block = new AGameObject(name, true);
    block->addComponent("ATransformComponent", new ATransformComponent(glm::vec2(0.0f),
                                                                       glm::vec2(sizeX, sizeY),
                                                                       0.0f, 1));
    block->transform = (ATransformComponent *)block->getComponent("ATransformComponent");
    block->addComponent("ASpriteComponent", new ASpriteComponent(sprite));
    return block;
}

AGameObject *APrefabs::generateObject()
{
    int NameSize = StringLength("Object_Gen");
    char *name = (char *)malloc(NameSize + 1);
    StringCopy(name, NameSize, "Object_Gen");
    name[NameSize] = '\0';

    AGameObject *block = new AGameObject(name);
    return block;
}

AGameObject *APrefabs::generateGuy()
{
    ASpritesheet *spritesheet = AAssetPool::getSpritesheet("characterSpritesheet");
    AGameObject *mario = generateSpriteObject(spritesheet->getSprite(16), 0.25f, 0.25f);
    
    AAnimationState *run = new AAnimationState();
    run->title = "run";
    float defaultFrameTime = 0.15f;
    run->addFrame(spritesheet->getSprite(16), defaultFrameTime);
    run->addFrame(spritesheet->getSprite(17), defaultFrameTime);
    run->addFrame(spritesheet->getSprite(18), defaultFrameTime);
    run->addFrame(spritesheet->getSprite(19), defaultFrameTime);
    run->addFrame(spritesheet->getSprite(20), defaultFrameTime);
    run->addFrame(spritesheet->getSprite(21), defaultFrameTime);
    run->addFrame(spritesheet->getSprite(22), defaultFrameTime);
    run->addFrame(spritesheet->getSprite(23), defaultFrameTime);
    run->setLoop(true);
    AStateMachine *stateMachine = new AStateMachine();
    stateMachine->addState(run);

    stateMachine->setDefaultStateTitle(run->title); 
    mario->addComponent("AStateMachine", stateMachine);
    return mario;
}

AGameObject *APrefabs::generateMario()
{
    ASpritesheet *spritesheet = AAssetPool::getSpritesheet("marioSpritesheet");
    AGameObject *mario = generateSpriteObject(spritesheet->getSprite(0), 0.25f, 0.25f);
    
    AAnimationState *run = new AAnimationState();
    run->title = "run";
    float defaultFrameTime = 0.23f;
    run->addFrame(spritesheet->getSprite(0), defaultFrameTime);
    run->addFrame(spritesheet->getSprite(2), defaultFrameTime);
    run->addFrame(spritesheet->getSprite(3), defaultFrameTime);
    run->addFrame(spritesheet->getSprite(2), defaultFrameTime);
    run->setLoop(true);
    AStateMachine *stateMachine = new AStateMachine();
    stateMachine->addState(run);

    stateMachine->setDefaultStateTitle(run->title); 
    mario->addComponent("AStateMachine", stateMachine);
    return mario;
}


AGameObject *APrefabs::generateQuestionBlock()
{
    ASpritesheet *spritesheet = AAssetPool::getSpritesheet("itemsSpritesheet");
    AGameObject *box = generateSpriteObject(spritesheet->getSprite(0), 0.25f, 0.25f);
    
    AAnimationState *flicker = new AAnimationState();
    flicker->title = "flicker";
    float defaultFrameTime = 0.23f;
    flicker->addFrame(spritesheet->getSprite(0), 0.57f);
    flicker->addFrame(spritesheet->getSprite(1), defaultFrameTime);
    flicker->addFrame(spritesheet->getSprite(2), defaultFrameTime);
    flicker->setLoop(true);
    AStateMachine *stateMachine = new AStateMachine();
    stateMachine->addState(flicker);

    stateMachine->setDefaultStateTitle(flicker->title); 
    box->addComponent("AStateMachine", stateMachine);
    return box;

}
