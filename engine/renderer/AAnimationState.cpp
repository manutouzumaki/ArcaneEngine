#include "AAnimationState.h"

AFrame::AFrame()
{
    this->sprite = nullptr;
    this->frameTime = 0.0f;
}

AFrame::AFrame(ASprite *sprite, float frameTime)
{
    this->sprite = sprite;
    this->frameTime = frameTime;
}

ASprite *AAnimationState::defaultSprite = nullptr;

AAnimationState::AAnimationState()
{
    timer = 0.0f;
    currentSprite = 0;
    doesLoop = false;
}

AAnimationState::~AAnimationState()
{
    free((void *)this->title);
    for(int i = 0; i < animationFrames.size(); ++i)
    {
        delete animationFrames[i];
    }
}

void AAnimationState::addFrame(ASprite *sprite, float frameTime)
{
    animationFrames.add(new AFrame(sprite, frameTime));
}

void AAnimationState::setLoop(bool value)
{
    this->doesLoop = value;
}

void AAnimationState::update(float dt)
{
    if(currentSprite < (int)animationFrames.size())
    {
        timer -= dt;
        if(timer <= 0.0f)
        {
            if(!(currentSprite == (int)animationFrames.size() - 1 && !doesLoop))
            {
                currentSprite = (currentSprite + 1) % (int)animationFrames.size();
            }
            timer = animationFrames[currentSprite]->frameTime;
        } 
    }
}

ASprite *AAnimationState::getCurrentSprite()
{
    if(currentSprite < (int)animationFrames.size())
    {
        return animationFrames[currentSprite]->sprite;
    }
    else
    {
        return defaultSprite;
    }
}

void AAnimationState::serialize(TiXmlElement *parent)
{
    TiXmlElement *state = new TiXmlElement("State");
    parent->LinkEndChild(state);
    TiXmlText *stateName = new TiXmlText(this->title);
    state->LinkEndChild(stateName);
    TiXmlElement *loop = new TiXmlElement("Loop");
    loop->SetAttribute("value", (int)this->doesLoop);
    state->LinkEndChild(loop);
    TiXmlElement *frames = new TiXmlElement("Frames");
    state->LinkEndChild(frames);
    for(int i = 0; i < animationFrames.size(); ++i)
    {
        TiXmlElement *frame = new TiXmlElement("Frame");
        TiXmlText *spritesheet = new TiXmlText(animationFrames[i]->sprite->getTileSheet());
        frame->LinkEndChild(spritesheet);
        TiXmlElement *spriteIndex = new TiXmlElement("SpriteIndex");
        spriteIndex->SetAttribute("value", animationFrames[i]->sprite->getIndex());
        frame->LinkEndChild(spriteIndex);
        TiXmlElement *frameTime = new TiXmlElement("FrameTime");
        frameTime->SetDoubleAttribute("value", animationFrames[i]->frameTime);
        frame->LinkEndChild(frameTime);
        frames->LinkEndChild(frame);
    }
    
}
