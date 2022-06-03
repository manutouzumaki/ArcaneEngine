#ifndef APHYSICS_H
#define APHYSICS_H

#include <box2d/box2d.h>
#include "AGameObject.h"
#include <glm/glm.hpp>

enum BodyType
{
    STATIC,
    DYNAMIC,
    KINEMATIC
};

class ACollider : public AComponent
{
protected:
    glm::vec2 offset;
public:
    glm::vec2 getOffset();
};

class ABoxCollider : public ACollider
{
private:
    glm::vec2 halfSize;
    glm::vec2 origin;
public:
    ABoxCollider();
    glm::vec2 getHalfSize();
    void setHalfSize(glm::vec2 halfSize);
    glm::vec2 getOrigin();

    void imgui() override;
    void serialize(TiXmlElement *parent) override;
};

class ACircleCollider : public ACollider
{
private:
    float radius;
public:
    ACircleCollider();
    float getRadius();
    void setRadius(float radius);

    void imgui() override;
    void serialize(TiXmlElement *parent) override;
};

class ARigidBody : public AComponent
{
private:
    glm::vec2 velocity;
    float angularDamping;
    float linearDamping;
    float mass;
    BodyType bodyType;
    bool fixedRotation;
    bool continuousCollision;
    b2Body *rawBody;
public:
    ARigidBody();
    glm::vec2 getVelocity();
    void setVelocity(glm::vec2 velocity);
    float getAngularDamping();
    void setAngularDamping(float angularDamping);
    float getLinearDamping();
    void setLinearDamping(float linearDamping);
    float getMass();
    void setMass(float mass);
    BodyType getBodyType();
    void setBodyType(BodyType bodyType);
    bool getFixedRotation();
    void setFixedRotation(bool value);
    bool getContinuousCollision();
    void setContinuousCollision(bool value);
    b2Body *getRawBody();
    void setRawBody(b2Body *body);

    void update(float dt) override;
    void imgui() override;
    void serialize(TiXmlElement *parent) override;    
};

class APhysics
{
private:
    b2Vec2 gravity;
    b2World world;
    float physicsTime;
    float physicsTimeStep;
    int velocityIterations;
    int positionIterations;
public:
    APhysics();
    ~APhysics();
    void addGameObject(AGameObject *go);
    void update(float dt);
};

#endif
