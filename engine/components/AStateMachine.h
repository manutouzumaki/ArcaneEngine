#ifndef ASTATEMACHINE_H
#define ASTATEMACHINE_H

#include "../util/AArray.h"
#include "../util/AHashmap.h"
#include "../renderer/AAnimationState.h"
#include "../src/AComponent.h"

class AStateMachine : public AComponent
{
public:
    AStateMachine();
    ~AStateMachine();
    AHashmap<AArray<const char *> *> stateTransfers;
    AArray<AAnimationState *> states;
    void addStateTrigger(AString from, AArray<const char *> *to);
    void addState(AAnimationState *state);
    void trigger(const char *target, int index);
    void start() override;
    void update(float dt) override;
    void editorUpdate(float dt) override;
    void imgui() override;
    void serialize(TiXmlElement *parent) override;
    void setDefaultStateTitle(const char *title);
private:
    AAnimationState *currentState;
    const char *defaultStateTitle;
};

#endif
