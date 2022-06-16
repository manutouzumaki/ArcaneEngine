#include "APrefabs.h"
#include <glm/glm.hpp>
#include "APhysics.h"
#include "../components/ASpriteComponent.h"
#include "../components/ATransformComponent.h"
#include "../components/AStateMachine.h"
#include "../components/APlayerController.h"
#include "../components/AQuestionBlock.h"
#include "../components/AGroundComponent.h"
#include "../components/ABlockCoin.h"
#include "../components/AMushroomAI.h"
#include "../components/AFlower.h"
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
    ASpritesheet *spritesheet = AAssetPool::getSpritesheet("marioAnimsSpritesheet");
    AGameObject *mario = generateSpriteObject(spritesheet->getSprite(0), 0.25f, 0.50f);

    float defaultFrameTime = 0.23f;
    
    // SMALL mario
    AAnimationState *idle = new AAnimationState();
    idle->title = "idle";
    idle->addFrame(spritesheet->getSprite(12), defaultFrameTime);
    
    AAnimationState *run = new AAnimationState();
    run->title = "run";
    run->addFrame(spritesheet->getSprite(13), defaultFrameTime);
    run->addFrame(spritesheet->getSprite(14), defaultFrameTime);
    run->addFrame(spritesheet->getSprite(15), defaultFrameTime);
    run->setLoop(true);

    AAnimationState *switchDirection = new AAnimationState();
    switchDirection->title = "switchDirection";
    switchDirection->addFrame(spritesheet->getSprite(16), defaultFrameTime);

    AAnimationState *jump = new AAnimationState();
    jump->title = "jump";
    jump->addFrame(spritesheet->getSprite(17), defaultFrameTime);

    // BIG mario
    AAnimationState *bigIdle = new AAnimationState();
    bigIdle->title = "bigIdle";
    bigIdle->addFrame(spritesheet->getSprite(0), defaultFrameTime);

    AAnimationState *bigRun = new AAnimationState();
    bigRun->title = "bigRun";
    bigRun->addFrame(spritesheet->getSprite(1), defaultFrameTime);
    bigRun->addFrame(spritesheet->getSprite(2), defaultFrameTime);
    bigRun->addFrame(spritesheet->getSprite(3), defaultFrameTime);
    bigRun->setLoop(true);

    AAnimationState *bigSwitchDirection = new AAnimationState();
    bigSwitchDirection->title = "bigSwitchDirection";
    bigSwitchDirection->addFrame(spritesheet->getSprite(4), defaultFrameTime);

    AAnimationState *bigJump = new AAnimationState();
    bigJump->title = "bigJump";
    bigJump->addFrame(spritesheet->getSprite(5), defaultFrameTime);

    // FIRE mario
    AAnimationState *fireIdle = new AAnimationState();
    fireIdle->title = "fireIdle";
    fireIdle->addFrame(spritesheet->getSprite(6), defaultFrameTime);

    AAnimationState *fireRun = new AAnimationState();
    fireRun->title = "fireRun";
    fireRun->addFrame(spritesheet->getSprite(7), defaultFrameTime);
    fireRun->addFrame(spritesheet->getSprite(8), defaultFrameTime);
    fireRun->addFrame(spritesheet->getSprite(9), defaultFrameTime);
    fireRun->setLoop(true);

    AAnimationState *fireSwitchDirection = new AAnimationState();
    fireSwitchDirection->title = "fireSwitchDirection";
    fireSwitchDirection->addFrame(spritesheet->getSprite(10), defaultFrameTime);

    AAnimationState *fireJump = new AAnimationState();
    fireJump->title = "fireJump";
    fireJump->addFrame(spritesheet->getSprite(11), defaultFrameTime);

    AStateMachine *stateMachine = new AStateMachine();
    // SMALL mario
    stateMachine->addState(idle);
    stateMachine->addState(run);
    stateMachine->addState(switchDirection);
    stateMachine->addState(jump);
    // BIG mario
    stateMachine->addState(bigIdle);
    stateMachine->addState(bigRun);
    stateMachine->addState(bigSwitchDirection);
    stateMachine->addState(bigJump);
    // FIRE mario
    stateMachine->addState(fireIdle);
    stateMachine->addState(fireRun);
    stateMachine->addState(fireSwitchDirection);
    stateMachine->addState(fireJump);

    stateMachine->setDefaultStateTitle(bigIdle->title); 
    mario->addComponent("AStateMachine", stateMachine);

    mario->addComponent("APlayerController", new APlayerController());

    return mario;
}


AGameObject *APrefabs::generateQuestionBlock()
{
    ASpritesheet *spritesheet = AAssetPool::getSpritesheet("itemsSpritesheet");
    AGameObject *box = generateSpriteObject(spritesheet->getSprite(0), 0.25f, 0.25f);
    
    float defaultFrameTime = 0.23f;
    
    AAnimationState *flicker = new AAnimationState();
    flicker->title = "flicker";
    flicker->addFrame(spritesheet->getSprite(0), 0.57f);
    flicker->addFrame(spritesheet->getSprite(1), defaultFrameTime);
    flicker->addFrame(spritesheet->getSprite(2), defaultFrameTime);
    flicker->setLoop(true);


    AAnimationState *inactive = new AAnimationState();
    inactive->title = "inactive";
    inactive->addFrame(spritesheet->getSprite(3), 0.1f);
    inactive->setLoop(false);

    AStateMachine *stateMachine = new AStateMachine();
    stateMachine->addState(flicker);
    stateMachine->addState(inactive);

    stateMachine->setDefaultStateTitle(flicker->title); 
    box->addComponent("AStateMachine", stateMachine);
    box->addComponent("AQuestionBlock", new AQuestionBlock());
    ARigidBody *rb = new ARigidBody();
    rb->setBodyType(STATIC);
    box->addComponent("ARigidBody", rb);
    ABoxCollider *boxCollider = new ABoxCollider();
    boxCollider->setHalfSize(glm::vec2(0.25f));
    box->addComponent("ABoxCollider", boxCollider);
    box->addComponent("AGroundComponent", new AGroundComponent());
    return box;

}

AGameObject *APrefabs::generateBlockCoin()
{
    ASpritesheet *spritesheet = AAssetPool::getSpritesheet("itemsSpritesheet");
    AGameObject *coin = generateSpriteObject(spritesheet->getSprite(7), 0.25f, 0.25f);
    
    float defaultFrameTime = 0.23f;
    
    AAnimationState *coinFlip = new AAnimationState();
    coinFlip->title = "coinFlip";
    coinFlip->addFrame(spritesheet->getSprite(7), 0.57f);
    coinFlip->addFrame(spritesheet->getSprite(8), defaultFrameTime);
    coinFlip->addFrame(spritesheet->getSprite(9), defaultFrameTime);
    coinFlip->setLoop(true);

    AStateMachine *stateMachine = new AStateMachine();
    stateMachine->addState(coinFlip);

    stateMachine->setDefaultStateTitle(coinFlip->title); 
    coin->addComponent("AStateMachine", stateMachine);
    //coin->addComponent("AQuestionBlock", new AQuestionBlock());
    coin->addComponent("ABlockCoin", new ABlockCoin());
    return coin;
}

AGameObject *APrefabs::generateMushroom()
{
    ASpritesheet *spritesheet = AAssetPool::getSpritesheet("itemsSpritesheet");
    AGameObject *mushroom = generateSpriteObject(spritesheet->getSprite(10), 0.25f, 0.25f);
    ARigidBody *rb = new ARigidBody();
    rb->setBodyType(DYNAMIC);
    rb->setFixedRotation(true);
    rb->setContinuousCollision(false);
    mushroom->addComponent("ARigidBody", rb);
    ACircleCollider *circleCollider = new ACircleCollider();
    circleCollider->setRadius(0.14f);
    mushroom->addComponent("ACircleCollider", circleCollider);
    mushroom->addComponent("AMushroomAI", new AMushroomAI());
    return mushroom;
}

AGameObject *APrefabs::generateFlower()
{
    ASpritesheet *spritesheet = AAssetPool::getSpritesheet("itemsSpritesheet");
    AGameObject *flower = generateSpriteObject(spritesheet->getSprite(20), 0.25f, 0.25f);
    ARigidBody *rb = new ARigidBody();
    rb->setBodyType(STATIC);
    rb->setFixedRotation(true);
    rb->setContinuousCollision(false);
    flower->addComponent("ARigidBody", rb);
    ACircleCollider *circleCollider = new ACircleCollider();
    circleCollider->setRadius(0.14f);
    flower->addComponent("ACircleCollider", circleCollider);
    flower->addComponent("AFlower", new AFlower());
    return flower;
}
