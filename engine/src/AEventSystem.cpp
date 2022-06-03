#include "AEventSystem.h"

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
}
