#ifndef AGAMEOBJECT_H
#define AGAMEOBJECT_H

#include "../util/AArray.h"
#include "../util/AHashmap.h"
#include "../util/AString.h"

#include "AComponent.h"
#include "ATransform.h"

class AGameObject
{
public:
    AGameObject(const char *name);
    AGameObject(const char *name, ATransform transform, int zIndex);
    ~AGameObject();

    void start();
    void update(float dt);

    void addComponent(AString componentName, AComponent *component);
    AComponent *getComponent(AString componentName);
    void removeComponent(AString componentName);
    ATransform transform;

    int getUID();
    int getZIndex();
    void imgui();
    void serialize(TiXmlElement *parent); 

    void setZIndex(int zIndex);
    void setSerializable(bool value);
    void setPickable(bool value);
    bool getPickable();
private:
    static unsigned int ID_COUNTER;
    unsigned int UID;
    const char *name;
    AArray<AComponent *> components;
    AHashmap<int> componentIndexMap;
    int zIndex;
    bool serializable;
    bool pickable;
};

#endif
