#ifndef ALEVELEDITORSCENE_H
#define ALEVELEDITORSCENE_H

#include "AScene.h"

class ALevelEditorScene : public AScene
{
public:
    ALevelEditorScene();
    void init() override;
    void update(float dt) override;
    ~ALevelEditorScene() override;
private:
    AGameObject *hero;
};

#endif
