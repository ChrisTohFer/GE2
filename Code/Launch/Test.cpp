#include "Platform/Window.h"

#include <thread>

int main()
{
    using namespace ge2;
    using namespace ge2::plat;

    Window window;
    while (window.IsOpen())
    {
        if (window.WasCloseButtonPressed())
            window.Close();

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(5ms);
    }
    
    return 0;
}