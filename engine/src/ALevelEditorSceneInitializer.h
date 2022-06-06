#ifndef ALEVELEDITORSCENE_H
#define ALEVELEDITORSCENE_H

#include "AScene.h"
#include "../renderer/ASpritesheet.h"

class ALevelEditorSceneInitializer : public ASceneInitializer
{
public:
    void init(AScene *scene) override;
    void imgui() override;
private:
    AGameObject *hero;
    ASpritesheet *sprites[2];
    AGameObject *levelEditor;
};

#endif
