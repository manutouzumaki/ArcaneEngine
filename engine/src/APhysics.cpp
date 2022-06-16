#include "APhysics.h"
#include "AWindow.h"
#include "AGameObject.h"
#include "../renderer/ADebugDraw.h"
#include "../editor/AImGui.h"
#include <glm/gtx/rotate_vector.hpp>
#include "../util/ADefines.h"

glm::vec2 ACollider::getOffset()
{
    return offset;
}

void ACollider::setOffset(glm::vec2 offset)
{
    this->offset = offset;
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

void ACircleCollider::editorUpdate(float dt)
{
    glm::vec2 center = gameObject->transform->position + offset;
    ADebugDraw::addCircle(center, radius, glm::vec3(0, 1, 0), 1);
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

////////////////////////////////////////////////////////////

// ARigidBody Component
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
    
    gravityScale = 1.0f;
    angularVelocity = 0.0f;
    friction = 0.1f;
    isSensor = false;

    rawBody = nullptr;
}
    
ARigidBody::ARigidBody(glm::vec2 velocity,
               float angularDamping,
               float linearDamping,
               float mass,
               BodyType bodyType,
               bool fixedRotation,
               bool continuousCollision,
               float gravityScale,
               float angularVelocity,
               float friction,
               bool isSensor)
{
    this->velocity = velocity;
    this->angularDamping = angularDamping;
    this->linearDamping = linearDamping;
    this->mass = mass;
    this->bodyType = bodyType;
    this->fixedRotation = fixedRotation;
    this->continuousCollision = continuousCollision;
    this->gravityScale = gravityScale;
    this->angularVelocity = angularVelocity;
    this->friction = friction;
    this->isSensor = isSensor;
    this->rawBody = nullptr;
}

void ARigidBody::update(float dt)
{
    if(rawBody)
    {
        if(bodyType == DYNAMIC || bodyType == KINEMATIC)
        {
            this->gameObject->transform->position.x = rawBody->GetPosition().x;
            this->gameObject->transform->position.y = rawBody->GetPosition().y;
            this->gameObject->transform->rotation = glm::degrees((float)rawBody->GetAngle());
            b2Vec2 vel = rawBody->GetLinearVelocity();
            velocity = glm::vec2(vel.x, vel.y);
        }
        else if(bodyType == STATIC)
        {
            rawBody->SetTransform(b2Vec2(gameObject->transform->position.x, gameObject->transform->position.y),
                                  gameObject->transform->rotation);
        }
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
    ImGui::Checkbox("FixedRotation", &fixedRotation);
    ImGui::Checkbox("ContinuousCollision", &continuousCollision);
    ImGuiDragFloat("GravityScale", &gravityScale);
    ImGuiDragFloat("AngularVelocity", &angularVelocity);
    ImGuiDragFloat("Friction", &friction);
    ImGui::Checkbox("IsSensor", &isSensor);
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
    TiXmlElement *gravityScale = new TiXmlElement("GravityScale");
    gravityScale->SetDoubleAttribute("value", this->gravityScale);
    root->LinkEndChild(gravityScale);
    TiXmlElement *angularVelocity = new TiXmlElement("AngularVelocity");
    angularVelocity->SetDoubleAttribute("value", this->angularVelocity);
    root->LinkEndChild(angularVelocity);
    TiXmlElement *friction = new TiXmlElement("Friction");
    friction->SetDoubleAttribute("value", this->friction);
    root->LinkEndChild(friction);
    TiXmlElement *isSensor = new TiXmlElement("IsSensor");
    isSensor->SetAttribute("value", (int)this->isSensor);
    root->LinkEndChild(isSensor);  
}

void ARigidBody::addVelocity(glm::vec2 force)
{
    if(rawBody)
    {
        rawBody->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
    }
}

void ARigidBody::addImpulse(glm::vec2 impulse)
{
    if(rawBody)
    {
        rawBody->ApplyLinearImpulseToCenter(b2Vec2(impulse.x, impulse.y), true);
    }
}

glm::vec2 ARigidBody::getVelocity()
{
    return velocity;
}

void ARigidBody::setVelocity(glm::vec2 velocity)
{
    this->velocity = velocity;
    if(rawBody)
    {
        rawBody->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
    }
}

void ARigidBody::setAngularVelocity(float angularVelocity)
{
    this->angularVelocity = angularVelocity;
    if(rawBody)
    {
        rawBody->SetAngularVelocity(angularVelocity);
    }
}

void ARigidBody::setGravityScale(float gravityScale)
{
    this->gravityScale = gravityScale;
    if(rawBody)
    {
        rawBody->SetGravityScale(gravityScale);
    }
}

void ARigidBody::setIsSensor()
{
    this->isSensor = true;
    if(rawBody)
    {
        AWindow::getScene()->getPhysics()->setIsSensor(this);
    }
}

void ARigidBody::setNotSensor()
{
    this->isSensor = false;
    if(rawBody)
    {
        AWindow::getScene()->getPhysics()->setNotSensor(this);
    }
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


RaycastInfo::RaycastInfo(AGameObject *go)
{
    fixture = nullptr;
    point = glm::vec2(0.0f);
    normal = glm::vec2(0.0f);
    fraction = 0.0f;
    hit = false;
    hitObject = nullptr;
    requestingObject = go;
}

float RaycastInfo::ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float fraction)
{
    if((size_t)fixture->GetUserData().pointer == (size_t)requestingObject)
    {
        return 1;
    }
    this->fixture = fixture;
    this->point = glm::vec2(point.x, point.y);
    this->normal = glm::vec2(normal.x, normal.y);
    this->fraction = fraction;
    this->hit = fraction != 0.0f;
    this->hitObject = (AGameObject *)fixture->GetUserData().pointer;
    return fraction;
}

APhysics::APhysics()
    : gravity(0.0f, -10.0f), world(gravity)
{
    physicsTime = 0.0f;
    physicsTimeStep = 1.0f / 60.0f;
    velocityIterations = 8;
    positionIterations = 3;
    world.SetContactListener(&contactListener);
}

APhysics::~APhysics()
{

}

static void AddBoxCollider(ARigidBody *rb, ABoxCollider *boxCollider)
{
    b2Body *body = rb->getRawBody();
    Assert(body != nullptr);

    b2PolygonShape shape;
    glm::vec2 halfSize = boxCollider->getHalfSize() * 0.5f;
    glm::vec2 offset = boxCollider->getOffset();
    glm::vec2 origin = boxCollider->getOrigin();
    shape.SetAsBox(halfSize.x, halfSize.y, b2Vec2(offset.x, offset.y), 0);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = rb->friction;
    fixtureDef.userData.pointer = (size_t)boxCollider->gameObject;
    fixtureDef.isSensor = rb->isSensor;
    body->CreateFixture(&fixtureDef);

}

static void AddCircleCollider(ARigidBody *rb, ACircleCollider *circleCollider)
{
    b2Body *body = rb->getRawBody();
    Assert(body != nullptr);

    b2CircleShape shape;
    shape.m_radius = circleCollider->getRadius();
    shape.m_p = b2Vec2(circleCollider->getOffset().x, circleCollider->getOffset().y);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = rb->friction;
    fixtureDef.userData.pointer = (size_t)circleCollider->gameObject;
    fixtureDef.isSensor = rb->isSensor;
    body->CreateFixture(&fixtureDef);
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
            bodyDef.userData.pointer = (size_t)go;
            bodyDef.bullet = rb->getContinuousCollision();
            bodyDef.gravityScale = rb->gravityScale;
            bodyDef.angularVelocity = rb->angularVelocity;
            switch (rb->getBodyType())
            {
                case STATIC: {bodyDef.type = b2_staticBody;} break;
                case DYNAMIC: {bodyDef.type = b2_dynamicBody;} break;
                case KINEMATIC: {bodyDef.type = b2_kinematicBody;} break;
            }
            b2Body *body = world.CreateBody(&bodyDef);
            b2MassData massData = {};
            massData.mass = rb->getMass();
            massData.center = bodyDef.position;
            body->SetMassData(&massData);
            rb->setRawBody(body);

            if(go->hasComponent("ABoxCollider"))
            {
                ABoxCollider *boxCollider = (ABoxCollider *)go->getComponent("ABoxCollider");
                AddBoxCollider(rb, boxCollider);
            }
            else if(go->hasComponent("ACircleCollider"))
            {
                ACircleCollider *circleCollider = (ACircleCollider *)go->getComponent("ACircleCollider");
                AddCircleCollider(rb, circleCollider);
            }
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

RaycastInfo APhysics::raycast(AGameObject *requestingObject, glm::vec2 point1 , glm::vec2 point2)
{
    RaycastInfo callback(requestingObject);
    b2Vec2 start = b2Vec2(point1.x, point1.y);
    b2Vec2 end = b2Vec2(point2.x, point2.y);
    world.RayCast(&callback, start, end);
    return callback;
}

void APhysics::setIsSensor(ARigidBody *rb)
{
    b2Body *body = rb->getRawBody(); 
    if(body)
    {
        b2Fixture *fixture = body->GetFixtureList();
        while(fixture)
        {
            fixture->SetSensor(true);
            fixture = fixture->GetNext();
        }
    }
}

void APhysics::setNotSensor(ARigidBody *rb)
{
    b2Body *body = rb->getRawBody(); 
    if(body)
    {
        b2Fixture *fixture = body->GetFixtureList();
        while(fixture)
        {
            fixture->SetSensor(false);
            fixture = fixture->GetNext();
        }
    }
}

glm::vec2 APhysics::getGravity()
{
    glm::vec2 gravity = glm::vec2(world.GetGravity().x, world.GetGravity().y);
    return gravity;
}

void APhysics::resetCircleCollider(ARigidBody *rb, ACircleCollider *circleCollider)
{
    b2Body *body = rb->getRawBody();
    if (body == nullptr) return;
    
    for(b2Fixture *fixture = body->GetFixtureList();
        fixture != nullptr;
        fixture = fixture->GetNext())
    {
        body->DestroyFixture(fixture);
    }
    AddCircleCollider(rb, circleCollider);
    body->ResetMassData();
}

void APhysics::resetBoxCollider(ARigidBody *rb, ABoxCollider *boxCollider)
{
    b2Body *body = rb->getRawBody();
    if (body == nullptr) return;

    for(b2Fixture *fixture = body->GetFixtureList();
        fixture != nullptr;
        fixture = fixture->GetNext())
    {
        body->DestroyFixture(fixture);
    }

    AddBoxCollider(rb, boxCollider);
    body->ResetMassData();
}
    
int APhysics::fixtureListSize(b2Body *body) {
        int size = 0;
        b2Fixture *fixture = body->GetFixtureList();
        while (fixture != nullptr)
        {
            size++;
            fixture = fixture->GetNext();
        }
        return size;
}

void AContactListener::BeginContact(b2Contact *contact)
{
    AGameObject *goA = (AGameObject *)contact->GetFixtureA()->GetUserData().pointer;
    AGameObject *goB = (AGameObject *)contact->GetFixtureB()->GetUserData().pointer;
    b2WorldManifold worldManifold;
    contact->GetWorldManifold(&worldManifold);
    glm::vec2 aNormal = glm::vec2(worldManifold.normal.x, worldManifold.normal.y);
    glm::vec2 bNormal = -aNormal;

    AArray<AComponent *> *components = goA->getAllComponents();
    for(int i = 0; i < components->size(); ++i)
    {
        (*components)[i]->beginCollision(goB, contact, aNormal);
    }

    components = goB->getAllComponents();
    for(int i = 0; i < components->size(); ++i)
    {
        (*components)[i]->beginCollision(goA, contact, bNormal);
    }
}

void AContactListener::EndContact(b2Contact *contact)
{
    AGameObject *goA = (AGameObject *)contact->GetFixtureA()->GetUserData().pointer;
    AGameObject *goB = (AGameObject *)contact->GetFixtureB()->GetUserData().pointer;
    b2WorldManifold worldManifold;
    contact->GetWorldManifold(&worldManifold);
    glm::vec2 aNormal = glm::vec2(worldManifold.normal.x, worldManifold.normal.y);
    glm::vec2 bNormal = -aNormal;

    AArray<AComponent *> *components = goA->getAllComponents();
    for(int i = 0; i < components->size(); ++i)
    {
        (*components)[i]->endCollision(goB, contact, aNormal);
    }

    components = goB->getAllComponents();
    for(int i = 0; i < components->size(); ++i)
    {
        (*components)[i]->endCollision(goA, contact, bNormal);
    }
}

void AContactListener::PreSolve(b2Contact *contact, const b2Manifold *oldManifold)
{
    AGameObject *goA = (AGameObject *)contact->GetFixtureA()->GetUserData().pointer;
    AGameObject *goB = (AGameObject *)contact->GetFixtureB()->GetUserData().pointer;
    b2WorldManifold worldManifold;
    contact->GetWorldManifold(&worldManifold);
    glm::vec2 aNormal = glm::vec2(worldManifold.normal.x, worldManifold.normal.y);
    glm::vec2 bNormal = -aNormal;

    AArray<AComponent *> *components = goA->getAllComponents();
    for(int i = 0; i < components->size(); ++i)
    {
        (*components)[i]->preSolve(goB, contact, aNormal);
    }

    components = goB->getAllComponents();
    for(int i = 0; i < components->size(); ++i)
    {
        (*components)[i]->preSolve(goA, contact, bNormal);
    }
}

void AContactListener::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse)
{
    AGameObject *goA = (AGameObject *)contact->GetFixtureA()->GetUserData().pointer;
    AGameObject *goB = (AGameObject *)contact->GetFixtureB()->GetUserData().pointer;
    b2WorldManifold worldManifold;
    contact->GetWorldManifold(&worldManifold);
    glm::vec2 aNormal = glm::vec2(worldManifold.normal.x, worldManifold.normal.y);
    glm::vec2 bNormal = -aNormal;

    AArray<AComponent *> *components = goA->getAllComponents();
    for(int i = 0; i < components->size(); ++i)
    {
        (*components)[i]->postSolve(goB, contact, aNormal);
    }

    components = goB->getAllComponents();
    for(int i = 0; i < components->size(); ++i)
    {
        (*components)[i]->postSolve(goA, contact, bNormal);
    }
}



