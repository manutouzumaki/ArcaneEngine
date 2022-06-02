#ifndef AGIZMOMANAGER_H
#define AGIZMOMANAGER_H

#include "../src/AComponent.h"
#include "../renderer/ASpritesheet.h"
#include "ATranslateGizmo.h"
#include "AScaleGizmo.h"

class AGizmoManager : public AComponent
{
private:
    ASpritesheet *spritesheet;
    ATranslateGizmo *translateGizmo;
    AScaleGizmo *scaleGizmo;

    int gizmoInUse = 0;
public:
    AGizmoManager(ASpritesheet *spritesheet);
    void start() override;
    void update(float dt) override;
}; 

#endif
