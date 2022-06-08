#ifndef AANIMATIONSTATE_H
#define AANIMATIONSTATE_H

#include "../util/AArray.h"
#include "ASprite.h"
#include <tinyxml.h>

class AFrame
{
public:
    AFrame();
    AFrame(ASprite *sprite, float frameTime);
    ASprite *sprite;
    float frameTime;
};

class AAnimationState
{
private:
    static ASprite *defaultSprite;
    float timer;
    int currentSprite;
public:
    bool doesLoop;
    AAnimationState();
    ~AAnimationState();
    const char *title;
    AArray<AFrame *> animationFrames;

    void addFrame(ASprite *sprite, float frameTime);
    void setLoop(bool value);
    void update(float dt);
    ASprite *getCurrentSprite();

    void serialize(TiXmlElement *parent);
};

#endif
