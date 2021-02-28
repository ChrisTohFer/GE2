#include "Platform/Window.h"

#include <thread>

int main()
{
    using namespace ge2;
    using namespace ge2::plat;

    Window window;
    int updateLengthMicroseconds = 5000;
    while (window.IsOpen())
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto messages = window.TakeMessages();

        if (messages.closeButtonPressed)
            window.Close();


        using namespace std::chrono_literals;
        std::this_thread::sleep_until(start + updateLengthMicroseconds * 1us);
    }
    
    return 0;
}