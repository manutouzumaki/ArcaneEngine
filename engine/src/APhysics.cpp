#include "APhysics.h"
#include "AGameObject.h"

#include "../editor/AImGui.h"
#include <glm/gtx/rotate_vector.hpp>

glm::vec2 ACollider::getOffset()
{
    return offset;
}

// ABoxCollider Component
////////////////////////////////////////////////////////////
ABoxCollider::ABoxCollider() 
    : halfSize(1.0f), origin(0.0f)
{

}

glm::vec2 ABoxCollider::getHalfSize()
{
    return halfSize;
}

void ABoxCollider::setHalfSize(glm::vec2 halfSize)
{
    this->halfSize = halfSize;
}

glm::vec2 ABoxCollider::getOrigin()
{
    return origin;
}

void ABoxCollider::imgui()
{
    ImGuiVector2("HalfSize", &halfSize, 1.0f);
    ImGuiVector2("Offset", &offset, 0.0f);
    ImGuiVector2("Origin", &origin, 0.0f);
}

void ABoxCollider::serialize(TiXmlElement *parent)
{

}
////////////////////////////////////////////////////////////


// ACircleCollider Component
////////////////////////////////////////////////////////////
ACircleCollider::ACircleCollider()
{
    radius = 1.0f;
}

float ACircleCollider::getRadius()
{
    return radius;
}

void ACircleCollider::setRadius(float radius)
{
    this->radius = radius;
}

void ACircleCollider::imgui()
{
    ImGuiDragFloat("Radius", &radius);
    ImGuiVector2("Offset", &offset, 0.0f);
}

void ACircleCollider::serialize(TiXmlElement *parent)
{

}
////////////////////////////////////////////////////////////

// ACircleCollider Component
////////////////////////////////////////////////////////////
ARigidBody::ARigidBody()
    : velocity(0.0f, 0.0f)
{
    angularDamping = 0.8f;
    linearDamping = 0.9f;
    mass = 0;
    bodyType = DYNAMIC;
    fixedRotation = false;
    continuousCollision = true;
    rawBody = nullptr;
}

void ARigidBody::update(float dt)
{
    if(rawBody)
    {
        this->gameObject->transform->position.x = rawBody->GetPosition().x;
        this->gameObject->transform->position.y = rawBody->GetPosition().y;
        this->gameObject->transform->rotation = glm::degrees(rawBody->GetAngle());
    }
}

void ARigidBody::imgui()
{
    ImGuiVector2("Velocioty", &velocity, 0.0f);
    ImGuiDragFloat("AngularDamping", &angularDamping);    
    ImGuiDragFloat("LinearDamping", &linearDamping);
    ImGuiDragFloat("Mass", &mass);
}

void ARigidBody::serialize(TiXmlElement *parent)
{

}

glm::vec2 ARigidBody::getVelocity()
{
    return velocity;
}

void ARigidBody::setVelocity(glm::vec2 velocity)
{
    this->velocity = velocity;
}

float ARigidBody::getAngularDamping()
{
    return angularDamping;
}

void ARigidBody::setAngularDamping(float angularDamping)
{
    this->angularDamping = angularDamping;
}

float ARigidBody::getLinearDamping()
{
    return linearDamping;
}

void ARigidBody::setLinearDamping(float linearDamping)
{
    this->linearDamping = linearDamping;
}

float ARigidBody::getMass()
{
    return mass;
}

void ARigidBody::setMass(float mass)
{
    this->mass = mass;
}

BodyType ARigidBody::getBodyType()
{
    return bodyType;
}

void ARigidBody::setBodyType(BodyType bodyType)
{
    this->bodyType = bodyType;
}

bool ARigidBody::getFixedRotation()
{
    return fixedRotation;
}

void ARigidBody::setFixedRotation(bool value)
{
    this->fixedRotation = value;
}

bool ARigidBody::getContinuousCollision()
{
    return continuousCollision;
}

void ARigidBody::setContinuousCollision(bool value)
{
    this->continuousCollision = value;
}

b2Body *ARigidBody::getRawBody()
{
    return rawBody;
}

void ARigidBody::setRawBody(b2Body *body)
{
    this->rawBody = body;
}
////////////////////////////////////////////////////////////


APhysics::APhysics()
    : gravity(0.0f, -10.0f), world(gravity)
{
    physicsTime = 0.0f;
    physicsTimeStep = 1.0f / 60.0f;
    velocityIterations = 8;
    positionIterations = 3;
}

void APhysics::addGameObject(AGameObject *go)
{
    if(go->hasComponent("ARigidBody"))
    {
        ARigidBody *rb = (ARigidBody *)go->getComponent("ARigidBody");
        if(!rb->getRawBody())
        {
            ATransformComponent *transform = go->transform;
            b2BodyDef bodyDef;
            bodyDef.angle = glm::radians(transform->rotation);
            bodyDef.position.Set(transform->position.x, transform->position.y);
            bodyDef.angularDamping = rb->getAngularDamping();
            bodyDef.linearDamping = rb->getLinearDamping();
            bodyDef.fixedRotation = rb->getFixedRotation();
            bodyDef.bullet = rb->getContinuousCollision();
            switch (rb->getBodyType())
            {
                case STATIC: {bodyDef.type = b2_staticBody;} break;
                case DYNAMIC: {bodyDef.type = b2_dynamicBody;} break;
                case KINEMATIC: {bodyDef.type = b2_kinematicBody;} break;
            }
            b2PolygonShape shape;
            if(go->hasComponent("ACircleCollider"))
            {
                ACircleCollider *circleCollider = (ACircleCollider *)go->getComponent("ACircleCollider");
                b2Vec2 points[20];
                float angle = 0;
                float angleIncrement = 360.0f/20.0f;
                for(int i = 0; i < 20; ++i)
                {
                    glm::vec2 point = glm::rotate(glm::vec2(circleCollider->getRadius(), 0.0f), glm::radians(angle));
                    points[i] = b2Vec2(point.x, point.y);
                    angle += angleIncrement;
                }
                shape.Set(points, 20);
            }
            else if(go->hasComponent("ABoxCollider"))
            {
                ABoxCollider *boxCollider = (ABoxCollider *)go->getComponent("ABoxCollider");
                glm::vec2 halfSize = boxCollider->getHalfSize() * 0.5f;
                glm::vec2 offset = boxCollider->getOffset();
                glm::vec2 origin = boxCollider->getOrigin();
                shape.SetAsBox(halfSize.x, halfSize.y, b2Vec2(origin.x, origin.y), 0);
                b2Vec2 pos = bodyDef.position;
                float xPos = pos.x + offset.x;
                float yPos = pos.y + offset.y;
                bodyDef.position.Set(xPos, yPos);
            }
            b2Body *body = world.CreateBody(&bodyDef);
            rb->setRawBody(body);
            body->CreateFixture(&shape, rb->getMass());
        }
    }
}

void APhysics::update(float dt)
{
    physicsTime += dt;
    if(physicsTime >= 0.0f)
    {
        physicsTime -= physicsTimeStep;
        world.Step(physicsTimeStep, velocityIterations, positionIterations);
    }
}
