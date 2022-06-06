#ifndef AGAMEOBJECT_H
#define AGAMEOBJECT_H

#include "../util/AArray.h"
#include "../util/AHashmap.h"
#include "../util/AString.h"

#include "AComponent.h"
#include "../components/ATransformComponent.h"

class AGameObject
{
public:
    AGameObject::AGameObject(const char *name, bool attachToScene = false);
    ~AGameObject();

    void start();
    void update(float dt);
    void editorUpdate(float dt);
    void destroy();

    void addComponent(AString componentName, AComponent *component);
    AComponent *getComponent(AString componentName);
    void removeComponent(AString componentName);
    bool hasComponent(AString componentName);
    ATransformComponent *transform;
    
    int getUID();
    void imgui();
    void serialize(TiXmlElement *parent); 

    void setSerializable(bool value);
    void setPickable(bool value);
    bool getPickable();
    bool getIsDead();

    static void resetUIDCounter();
private:
    static unsigned int ID_COUNTER;
    unsigned int UID;
    const char *name;
    AArray<AComponent *> components;
    AHashmap<int> componentIndexMap;
    bool serializable;
    bool pickable;
    bool isDead;
};

#endif
