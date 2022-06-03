#ifndef AEVENTSYSTEM_H
#define AEVENTSYSTEM_H

#include "AGameObject.h"
#include "../util/AArray.h"

enum EventType
{
    GAME_ENGINE_START_PLAY,
    GAME_ENGINE_STOP_PLAY,
    SAVE_LEVEL,
    LOAD_LEVEL,
    USER_EVENT
};

class AEvent
{
public:
    EventType type;
};

class AObserver
{
public:
    virtual void onNotify(AGameObject *obj, AEvent *event) = 0;
};

class AEventSystem
{
private:
    static AArray<AObserver *> observers;
public:
    static void addObserver(AObserver *observer);
    static void notify(AGameObject *obj, AEvent *event);

};

#endif
