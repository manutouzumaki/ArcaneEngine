#ifndef ASCENEINITIALIZER_H
#define ASCENEINITIALIZER_H

class AScene;

class ASceneInitializer
{
public:
    virtual void init(AScene *scene) = 0;
    virtual void imgui() = 0;
};

#endif
