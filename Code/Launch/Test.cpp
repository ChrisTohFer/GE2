#include "Platform/Window.h"

#include <thread>
#include <iostream>

int main()
{
    using namespace ge2;
    using namespace ge2::plat;

    Window window;
    while (window.IsOpen())
    {
        auto messages = window.TakeMessages();

        //TEMPORARY CODE vvv
        if (messages.keyEvents.size() != 0)
            std::cout << "KeyEvents:\t" << messages.keyEvents.size() << std::endl;

        if (messages.mouseEvents.size() != 0)
            std::cout << "MouseEvents:\t" << messages.mouseEvents.size() << std::endl;
        //TEMPORARY CODE ^^^

        if (messages.closeButtonPressed)
            window.Close();

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(5ms);
    }
    
    return 0;
}