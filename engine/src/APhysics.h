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
    void setOffset(glm::vec2 offset);
};

class ABoxCollider : public ACollider
{
private:
    glm::vec2 halfSize;
    glm::vec2 origin;
public:
    ABoxCollider();
    ABoxCollider(glm::vec2 offset, glm::vec2 halfSize, glm::vec2 origin);
    glm::vec2 getHalfSize();
    void setHalfSize(glm::vec2 halfSize);
    glm::vec2 getOrigin();
    void editorUpdate(float dt) override;
    void imgui() override;
    void serialize(TiXmlElement *parent) override;
};

class ACircleCollider : public ACollider
{
private:
    float radius;
public:
    ACircleCollider();
    ACircleCollider(glm::vec2 offset, float radius);
    float getRadius();
    void setRadius(float radius);

    void editorUpdate(float dt) override;
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
    float gravityScale;
    float angularVelocity;
    float friction;
    bool isSensor;
    ARigidBody();
    ARigidBody(glm::vec2 velocity,
               float angularDamping,
               float linearDamping,
               float mass,
               BodyType bodyType,
               bool fixedRotation,
               bool continuousCollision,
               float gravityScale,
               float angularVelocity,
               float friction,
               bool isSensor);
    void addVelocity(glm::vec2 force);
    void addImpulse(glm::vec2 impulse);
    glm::vec2 getVelocity();
    void setVelocity(glm::vec2 velocity);

    void setAngularVelocity(float angularVelocity);
    void setGravityScale(float gravityScale);
    void setIsSensor();
    void setNotSensor();

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

class RaycastInfo : public b2RayCastCallback
{
public:
    b2Fixture *fixture;
    glm::vec2 point;
    glm::vec2 normal;
    float fraction;
    bool hit;
    AGameObject *hitObject;
    RaycastInfo(AGameObject *go);
    float ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float fraction) override;

private:
    AGameObject *requestingObject;
};

class AContactListener : public b2ContactListener
{
public:
    void BeginContact(b2Contact *contact) override;
    void EndContact(b2Contact *contact) override;
    void PreSolve(b2Contact *contact, const b2Manifold *oldManifold) override;
    void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse) override;
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
    AContactListener contactListener;
    int fixtureListSize(b2Body *body);
public:
    APhysics();
    ~APhysics();
    void addGameObject(AGameObject *go);
    void destroyGameObject(AGameObject *go);
    void update(float dt);
    RaycastInfo raycast(AGameObject *requestingObject, glm::vec2 point1 , glm::vec2 point2);
    void setIsSensor(ARigidBody *rb);
    void setNotSensor(ARigidBody *rb);
    glm::vec2 getGravity();
    void resetCircleCollider(ARigidBody *rb, ACircleCollider *circleCollider);
    void resetBoxCollider(ARigidBody *rb, ABoxCollider *boxCollider);
};

#endif
