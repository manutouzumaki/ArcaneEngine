#include "AStateMachine.h" 
#include "ASpriteComponent.h"
#include "../src/AGameObject.h"

#include "../editor/AImGui.h"

AStateMachine::AStateMachine()
{
    currentState = nullptr;
    defaultStateTitle = "";
}

AStateMachine::~AStateMachine()
{
    for(int i = 0; i < states.size(); ++i)
    {
        delete states[i];
    }
}

void AStateMachine::addStateTrigger(AString from, AArray<const char *> *to)
{
    stateTransfers.add(from, to); 
}

void AStateMachine::addState(AAnimationState *state)
{
    states.add(state);
}

void AStateMachine::trigger(const char *target, int index)
{
    if(currentState)
    {
        AArray<const char *> *currentTargets = stateTransfers[currentState->title];
        for(int i = 0; i < currentTargets->size(); ++i)
        {
            const char *currentTarget = (*currentTargets)[i];
            if(StringCompare(target, currentTarget, StringLength(currentTarget)))
            {
                currentState = states[index]; 
                return;
            }
        }
    }
}

void AStateMachine::start()
{
    for(int i = 0; i < states.size(); ++i)
    {
        AAnimationState *state = states[i];
        if(StringCompare(state->title, defaultStateTitle, StringLength(state->title)))
        {
            currentState = state;
            break;
        }
    }
}

void AStateMachine::update(float dt)
{
    if(currentState)
    {
        currentState->update(dt);
        ASpriteComponent *sprite = (ASpriteComponent *)gameObject->getComponent("ASpriteComponent");
        sprite->setSprite(currentState->getCurrentSprite());
    }
}

void AStateMachine::editorUpdate(float dt)
{
    if(currentState)
    {
        currentState->update(dt);
        ASpriteComponent *sprite = (ASpriteComponent *)gameObject->getComponent("ASpriteComponent");
        sprite->setSprite(currentState->getCurrentSprite());
    }
}

void AStateMachine::imgui()
{
    for(int i = 0; i < states.size(); ++i)
    {
        AAnimationState *state = states[i];
        ImGui::Checkbox("loop", &state->doesLoop);
        for(int j = 0; j < state->animationFrames.size(); ++j)
        {
            ImGui::PushID(j);
            AFrame *frame = state->animationFrames[j];
            ImGuiDragFloat("frameTime", &frame->frameTime);
            ImGui::PopID(); 
        }   
    }
}

void AStateMachine::serialize(TiXmlElement *parent)
{
    TiXmlElement *root = new TiXmlElement("AStateMachine");
    parent->LinkEndChild(root);
    TiXmlText *type = new TiXmlText("AStateMachine");
    root->LinkEndChild(type);
    TiXmlElement *states = new TiXmlElement("States");
    root->LinkEndChild(states);
    for(int i = 0; i < this->states.size(); ++i)
    {
        this->states[i]->serialize(states);
    }
}

void AStateMachine::setDefaultStateTitle(const char *title)
{
    for(int i = 0; i < states.size(); ++i)
    {
        AAnimationState *state = states[i];
        if(StringCompare(state->title, title, StringLength(title)))
        {
            this->defaultStateTitle = title; 
            if(currentState == nullptr)
            {
                currentState = state;
                return;
            }
        }
    }
}


