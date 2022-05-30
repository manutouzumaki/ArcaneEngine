#ifndef ALEVELSCENE_H
#define ALEVELSCENE_H

#include "AScene.h"

class ALevelScene : public AScene
{
public:
    ALevelScene();
    void update(float dt) override;
    void render() override;
};

#endif
