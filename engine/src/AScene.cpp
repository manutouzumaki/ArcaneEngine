#include "AScene.h"


AScene::AScene(ASceneInitializer *sceneInitializer)
{
    camera = nullptr;
    isRunning = false;
    this->sceneInitializer = sceneInitializer;
    this->renderer = new ARenderer();
    this->physics = new APhysics();
}

AScene::~AScene()
{
    if(physics)
    {
        delete physics;
        physics = nullptr;
    }
    if(renderer)
    {
        delete renderer;
        renderer = nullptr;
    }
    if(sceneInitializer)
    {
        delete sceneInitializer;
        sceneInitializer = nullptr;
    }
    if(camera)
    {
        delete camera;
        camera = nullptr;
    }
    for(int i = 0; i < gameObjects.size(); ++i)
    {
        if(gameObjects[i]) delete gameObjects[i];
    }
}

void AScene::init()
{
    camera = new ACamera(glm::vec3(0, 0, 20));
    this->sceneInitializer->init(this);
}

void AScene::start()
{
    for(int i = 0; i < gameObjects.size(); ++i)
    {
        gameObjects[i]->start();
        renderer->add(gameObjects[i]);
        physics->addGameObject(gameObjects[i]);
    }
    isRunning = true;
}

void AScene::update(float dt)
{
    physics->update(dt);
    for(int i = 0; i < gameObjects.size(); ++i)
    {
        gameObjects[i]->update(dt);
        if(gameObjects[i]->getIsDead())
        {
            this->renderer->destroyGameObject(gameObjects[i]);
            this->physics->destroyGameObject(gameObjects[i]);
            delete gameObjects[i];
            gameObjects.remove(i);
            i--;
        }
    }

    for(int i = 0; i < pendingObjects.size(); ++i)
    {
        AGameObject *gameObject = pendingObjects[i];
        gameObjects.add(gameObject);
        gameObject->start();
        renderer->add(gameObject);
        physics->addGameObject(gameObject);
        pendingObjects.remove(i);
        i--;
    }

    for(int i = 0; i < resetColliders.size(); ++i)
    {
        AGameObject *gameObject = resetColliders[i];
        ARigidBody *rb = (ARigidBody *)gameObject->getComponent("ARigidBody");
        if(gameObject->hasComponent("ACircleCollider"))
        {
            ACircleCollider *circleCollider = (ACircleCollider *)gameObject->getComponent("ACircleCollider");
            physics->resetCircleCollider(rb, circleCollider);
        }
        else if(gameObject->hasComponent("ABoxCollider"))
        {
            ABoxCollider *boxCollider = (ABoxCollider *)gameObject->getComponent("ABoxCollider");
            physics->resetBoxCollider(rb, boxCollider);
        }
        resetColliders.remove(i);
        i--;
    }
}

void AScene::editorUpdate(float dt)
{
    for(int i = 0; i < gameObjects.size(); ++i)
    {
        gameObjects[i]->editorUpdate(dt);

        if(gameObjects[i]->getIsDead())
        {
            this->renderer->destroyGameObject(gameObjects[i]);
            this->physics->destroyGameObject(gameObjects[i]);
        }
    }
    for(int i = 0; i < pendingObjects.size(); ++i)
    {
        AGameObject *gameObject = pendingObjects[i];
        gameObjects.add(gameObject);
        gameObject->start();
        renderer->add(gameObject);
        physics->addGameObject(gameObject);
        pendingObjects.remove(i);
        i--;
    }
}

void AScene::render()
{
    this->renderer->render();
}

void AScene::imgui()
{
    sceneInitializer->imgui();
}

void AScene::save()
{
    TiXmlDocument doc;
    TiXmlDeclaration *decl = new TiXmlDeclaration("1.0f", "", "");
    doc.LinkEndChild(decl);
    TiXmlElement *root = new TiXmlElement("GameObjects");
    doc.LinkEndChild(root);
    for(int i = 0; i < gameObjects.size(); ++i)
    {
        gameObjects[i]->serialize(root);
    }
    doc.SaveFile("../assets/saves/tinyXmlTest.xml"); 
}

void AScene::destroy()
{
    for(int i = 0; i < gameObjects.size(); ++i)
    {
        gameObjects[i]->destroy();
    }
}

void AScene::addGameObject(AGameObject *gameObject)
{
    if(!isRunning)
    {
        gameObjects.add(gameObject);
    }
    else
    {
        pendingObjects.add(gameObject);

    }
}

void AScene::addResetColliders(AGameObject *gameObject)
{
    resetColliders.add(gameObject);
}

AGameObject *AScene::getGameObjectByIndex(int index)
{
    return gameObjects[index];
}

ACamera *AScene::getCamera()
{
    return camera;
}

APhysics *AScene::getPhysics()
{
    return physics;
}

AArray<AGameObject *> *AScene::getGameObjects()
{
    return &gameObjects;
}
