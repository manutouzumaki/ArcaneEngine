#include "AGameObject.h"
#include "AFactory.h"
#include <imgui.h>
#include <tinyxml.h>

unsigned int AGameObject::ID_COUNTER = 0;

void AGameObject::resetUIDCounter()
{
    ID_COUNTER = 0;
}

AGameObject::AGameObject(const char *name, bool attachToScene)
{
    this->isDead = false;
    if(attachToScene)
    {
        this->name = name;
        this->serializable = true;
        this->pickable = true;
        this->UID = ID_COUNTER++;
    }
    else
    {
        this->name = name;
        this->serializable = false;
        this->pickable = false;
        this->UID = ID_COUNTER++;
    }
}

AGameObject::~AGameObject()
{
    free((void *)this->name);
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

bool AGameObject::hasComponent(AString componentName)
{
    return componentIndexMap.isKeyInHashmap(componentName);
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

void AGameObject::editorUpdate(float dt)
{
    for(int i = 0; i < components.size(); ++i)
    {
        components[i]->editorUpdate(dt);
    }
}

void AGameObject::destroy()
{
    this->isDead = true;
    for(int i = 0; i < components.size(); ++i)
    {
        components[i]->destroy();
    }

}

int AGameObject::getUID()
{
    return UID;
}

void AGameObject::setUID(int value)
{
    this->UID = value;
}

void AGameObject::setSerializable(bool value)
{
    this->serializable = value;
}

void AGameObject::setPickable(bool value)
{
    this->pickable = value;
}

bool AGameObject::getPickable()
{
    return this->pickable;
}

bool AGameObject::getIsDead()
{
    return this->isDead;
}

bool AGameObject::getSerializable()
{
    return this->serializable;
}

AGameObject *AGameObject::copy()
{
    // TODO: come up with cleaner solution
    TiXmlDocument doc;
    TiXmlDeclaration *decl = new TiXmlDeclaration("1.0f", "", "");
    doc.LinkEndChild(decl);
    TiXmlElement *root = new TiXmlElement("GameObjects");
    doc.LinkEndChild(root);
    this->serialize(root);
    TiXmlElement *gameObjectsNode = doc.FirstChildElement();
    for(TiXmlElement *gameObject = gameObjectsNode->FirstChildElement();
        gameObject != nullptr;
        gameObject = gameObject->NextSiblingElement())
    {
        return AFactory::CreateGameObject(gameObject);
    }
    return nullptr;
}

void AGameObject::imgui()
{
    for(int i = 0; i < components.size(); ++i)
    {
        if(ImGui::CollapsingHeader(components[i]->name.get()))
        {
            components[i]->imgui();
        }
    }
}

void AGameObject::serialize(TiXmlElement *parent)
{
    if(serializable && !isDead)
    {
        TiXmlElement *root = new TiXmlElement("AGameObject");
        parent->LinkEndChild(root);
        TiXmlText *name = new TiXmlText(this->name);
        root->LinkEndChild(name);
        TiXmlElement *components = new TiXmlElement("Components");
        for(int i = 0; i < this->components.size(); ++i)
        {
            this->components[i]->serialize(components);
        }
        root->LinkEndChild(components);
    }  
}

AArray<AComponent *> *AGameObject::getAllComponents()
{
    return &this->components;
}




