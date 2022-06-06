#ifndef AGRIDLINES_H
#define AGRIDLINES_H

#include "../src/AComponent.h"
class AGridLines : public AComponent
{
public:
    void editorUpdate(float dt) override;
};

#endif
