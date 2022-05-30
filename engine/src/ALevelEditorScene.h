#ifndef ALEVELEDITORSCENE_H
#define ALEVELEDITORSCENE_H

#include "AScene.h"
#include "../renderer/ASpritesheet.h"
#include "../components/AMouseControlComponent.h"

class ALevelEditorScene : public AScene
{
public:
    ALevelEditorScene();
    void init() override;
    void update(float dt) override;
    void render() override;
    void close() override;
    void imgui() override;
    ~ALevelEditorScene() override;
private:
    AGameObject *hero;
    ASpritesheet *sprites[2];
    AGameObject *levelEditor;
    //AMouseControlComponent mouseControls;
};

#endif
