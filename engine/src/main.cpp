#include "AWindow.h"
#include "AMouseListener.h"
#include "AKeyListener.h"


int main()
{
    // Create window
    AWindow *window = AWindow::get();
    // update the game

    window->run();
    // free subsystems
    AMouseListener::free();
    AKeyListener::free();
    AWindow::free();

    return 0;
}
