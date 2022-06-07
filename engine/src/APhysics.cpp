#include "APhysics.h"
#include "AGameObject.h"
#include "../renderer/ADebugDraw.h"
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

ABoxCollider::ABoxCollider(glm::vec2 offset, glm::vec2 halfSize, glm::vec2 origin)
{
    this->offset = offset;
    this->halfSize = halfSize;
    this->origin = origin;
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

void ABoxCollider::editorUpdate(float dt)
{
    glm::vec2 center = gameObject->transform->position + offset;
    ADebugDraw::addBox(center, halfSize, glm::radians(gameObject->transform->rotation), glm::vec3(0, 1, 0), 1);
}

void ABoxCollider::imgui()
{
    ImGuiVector2("HalfSize", &halfSize, 1.0f);
    ImGuiVector2("Offset", &offset, 0.0f);
    ImGuiVector2("Origin", &origin, 0.0f);
}

void ABoxCollider::serialize(TiXmlElement *parent)
{
    TiXmlElement *root = new TiXmlElement("ABoxCollider");
    parent->LinkEndChild(root);
    TiXmlText *type = new TiXmlText("ABoxCollider");
    root->LinkEndChild(type);
    TiXmlElement *offset = new TiXmlElement("Offset");
    offset->SetDoubleAttribute("x", this->offset.x);
    offset->SetDoubleAttribute("y", this->offset.y);
    root->LinkEndChild(offset);  
    TiXmlElement *halfSize = new TiXmlElement("HalfSize");
    halfSize->SetDoubleAttribute("x", this->halfSize.x);
    halfSize->SetDoubleAttribute("y", this->halfSize.y);
    root->LinkEndChild(halfSize); 
    TiXmlElement *origin = new TiXmlElement("Origin");
    origin->SetDoubleAttribute("x", this->origin.x);
    origin->SetDoubleAttribute("y", this->origin.y);
    root->LinkEndChild(origin); 
}
////////////////////////////////////////////////////////////


// ACircleCollider Component
////////////////////////////////////////////////////////////
ACircleCollider::ACircleCollider()
{
    radius = 1.0f;
}

ACircleCollider::ACircleCollider(glm::vec2 offset, float radius)
{
    this->offset = offset;
    this->radius = radius;
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
    TiXmlElement *root = new TiXmlElement("ACircleCollider");
    parent->LinkEndChild(root);
    TiXmlText *type = new TiXmlText("ACircleCollider");
    root->LinkEndChild(type);
    TiXmlElement *offset = new TiXmlElement("Offset");
    offset->SetDoubleAttribute("x", this->offset.x);
    offset->SetDoubleAttribute("y", this->offset.y);
    root->LinkEndChild(offset);  
    TiXmlElement *radius = new TiXmlElement("Radius");
    radius->SetDoubleAttribute("value", this->radius);
    root->LinkEndChild(radius);
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
    
ARigidBody::ARigidBody(glm::vec2 velocity,
               float angularDamping,
               float linearDamping,
               float mass,
               BodyType bodyType,
               bool fixedRotation,
               bool continuousCollision)
{
    this->velocity = velocity;
    this->angularDamping = angularDamping;
    this->linearDamping = linearDamping;
    this->mass = mass;
    this->bodyType = bodyType;
    this->fixedRotation = fixedRotation;
    this->continuousCollision = continuousCollision;
    this->rawBody = nullptr;
}

void ARigidBody::update(float dt)
{
    if(rawBody)
    {
        this->gameObject->transform->position.x = rawBody->GetPosition().x;
        this->gameObject->transform->position.y = rawBody->GetPosition().y;
        this->gameObject->transform->rotation = glm::degrees((float)rawBody->GetAngle());
    }
}

void ARigidBody::imgui()
{
    ImGuiVector2("Velocioty", &velocity, 0.0f);
    ImGuiDragFloat("AngularDamping", &angularDamping);    
    ImGuiDragFloat("LinearDamping", &linearDamping);
    ImGuiDragFloat("Mass", &mass);
    const char *enumValues[3] = 
    {
        "STATIC",
        "DYNAMIC",
        "KINEMATIC"
    };
    int index = (BodyType)bodyType;
    if(ImGui::Combo("bodyType", &index, enumValues, 3))
    {
        bodyType = (BodyType)index; 
    }
    ImGui::Checkbox("fixedRotation", &fixedRotation);
    ImGui::Checkbox("ContinuousCollision", &continuousCollision);
}

void ARigidBody::serialize(TiXmlElement *parent)
{
    TiXmlElement *root = new TiXmlElement("ARigidBody");
    parent->LinkEndChild(root);
    TiXmlText *type = new TiXmlText("ARigidBody");
    root->LinkEndChild(type);
    TiXmlElement *velocity = new TiXmlElement("Velocity");
    velocity->SetDoubleAttribute("x", this->velocity.x);
    velocity->SetDoubleAttribute("y", this->velocity.y);
    root->LinkEndChild(velocity);
    TiXmlElement *angularDamping = new TiXmlElement("AngularDamping");
    angularDamping->SetDoubleAttribute("value", this->angularDamping);
    root->LinkEndChild(angularDamping);
    TiXmlElement *linearDamping = new TiXmlElement("LinearDamping");
    linearDamping->SetDoubleAttribute("value", this->linearDamping);
    root->LinkEndChild(linearDamping);
    TiXmlElement *mass = new TiXmlElement("Mass");
    mass->SetDoubleAttribute("value", this->mass);
    root->LinkEndChild(mass);
    TiXmlElement *bodyType = new TiXmlElement("BodyType");
    bodyType->SetAttribute("value", this->bodyType);
    root->LinkEndChild(bodyType);
    TiXmlElement *fixedRotation = new TiXmlElement("FixedRotation");
    fixedRotation->SetAttribute("value", (int)this->fixedRotation);
    root->LinkEndChild(fixedRotation);
    TiXmlElement *continuousCollision = new TiXmlElement("ContinuousCollision");
    continuousCollision->SetAttribute("value", (int)this->continuousCollision);
    root->LinkEndChild(continuousCollision);    
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

APhysics::~APhysics()
{

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
            rb->setRawBody(world.CreateBody(&bodyDef));

            b2FixtureDef fixtureDef;
            fixtureDef.shape = &shape;
            fixtureDef.density = 1.0f;
            fixtureDef.friction = 0.3f;
            rb->getRawBody()->CreateFixture(&fixtureDef);
        }
    }
}


void APhysics::destroyGameObject(AGameObject *go)
{
    if(go->hasComponent("ARigidBody"))
    {
        ARigidBody *rb = (ARigidBody *)go->getComponent("ARigidBody");
        if(rb->getRawBody())
        {
            world.DestroyBody(rb->getRawBody());
            rb->setRawBody(nullptr);
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
