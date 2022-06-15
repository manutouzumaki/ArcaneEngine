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
    AArray<AAnimationState *> states;
    void addState(AAnimationState *state);
    void trigger(const char *target);
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
