#include "AEventSystem.h"

AEvent::AEvent()
{
    this->type = USER_EVENT;
}

AEvent::AEvent(EventType type)
{
    this->type = type;
}

AArray<AObserver *> AEventSystem::observers;

void AEventSystem::addObserver(AObserver *observer)
{
    observers.add(observer);
}

void AEventSystem::notify(AGameObject *obj, AEvent *event)
{
    for(int i = 0; i < observers.size(); ++i)
    {
        observers[i]->onNotify(obj, event);
    }
    if(event) delete event;
}
