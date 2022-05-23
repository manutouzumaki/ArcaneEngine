#include "AGameObject.h"

AGameObject::AGameObject(const char *name)
{
    this->name = name;
    this->zIndex = 0;
}

AGameObject::AGameObject(const char *name, ATransform transform, int zIndex)
{
    this->name = name;
    this->transform = transform;
    this->zIndex = zIndex;
}

AGameObject::~AGameObject()
{
    for(int i = 0; i < components.size(); ++i)
    {
        delete components[i];
    }
}

void AGameObject::addComponent(AString componentName, AComponent *component)
{
    componentIndexMap.add(componentName, (int)components.size());
    component->gameObject = this;
    component->name = componentName;
    components.add(component);
}

AComponent *AGameObject::getComponent(AString componentName)
{
    int componentIndex = componentIndexMap[componentName];
    return components[componentIndex];
}

void AGameObject::removeComponent(AString componentName)
{
    int componentIndex = componentIndexMap[componentName];
    int lastComponentIndex = (int)components.size() - 1;
    componentIndexMap[components[lastComponentIndex]->name] = componentIndex;
    componentIndexMap.remove(componentName);
    delete components[componentIndex];
    components[componentIndex] = components[lastComponentIndex];
    components[lastComponentIndex] = nullptr;
    components.occupied = components.occupied - 1;
}

void AGameObject::start()
{
    for(int i = 0; i < components.size(); ++i)
    {
        components[i]->start();
    }
}

void AGameObject::update(float dt)
{
    for(int i = 0; i < components.size(); ++i)
    {
        components[i]->update(dt);
    }
}


int AGameObject::getZIndex()
{
    return zIndex;
}
