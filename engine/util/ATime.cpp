#include "ATime.h"

#include <GLFW/glfw3.h>

float ATime::timeStarted = (float)glfwGetTime();

float ATime::getTime()
{
    return (float)(glfwGetTime() - timeStarted);
}
