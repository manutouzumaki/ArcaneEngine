#include "AGridLines.h"
#include "../src/AWindow.h"
#include "../util/ADefines.h"
#include "../renderer/ADebugDraw.h"
#include <glm/glm.hpp>

void AGridLines::update(float dt)
{
    glm::vec2 cameraPos = AWindow::getScene()->getCamera()->position;
    glm::vec2 projectionSize = AWindow::getScene()->getCamera()->projectionSize;
    int firstX = ((int)(cameraPos.x / GRID_WIDTH) - 1) * GRID_WIDTH;
    int firstY = ((int)(cameraPos.y / GRID_HEIGHT) - 1) * GRID_HEIGHT;
    int numVLines = (int)(projectionSize.x / GRID_WIDTH) + 2;
    int numHLines = (int)(projectionSize.y / GRID_HEIGHT) + 2;

    int height = (int)projectionSize.y + GRID_HEIGHT * 2;
    int width = (int)projectionSize.x + GRID_WIDTH * 2;
    
    int maxLines = (numVLines > numHLines) ? numVLines : numHLines;
    for(int i = 0; i < maxLines; ++i)
    {
        int x = firstX + (GRID_WIDTH * i);
        int y = firstY + (GRID_HEIGHT * i);

        if(i < numVLines)
        {
            ADebugDraw::addLine(glm::vec2(x , firstY), glm::vec2(x, firstY + height), glm::vec3(0.2, 0.2, 0.2)); 
        }

        if(i < numHLines)
        {
            ADebugDraw::addLine(glm::vec2(firstX, y), glm::vec2(firstX + width, y), glm::vec3(0.2, 0.2, 0.2)); 
        }
    }
}
