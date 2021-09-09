#include "Editor.h"

#include "Window.h" //Editor/window

#include "Graphics/Core.h"
#include "Platform/ImguiHelpers.h"
#include "Platform/Window.h"

#include "imgui.h"

#include <chrono>
#include <thread>

namespace ge2
{
    void EditorLoop(plat::Window& window)
    {
        using namespace ge2;

        Input input;
        WindowMessages messages;
        int updateLengthMicroseconds = 1000000 / 60;
        while (!messages.closeButtonPressed)
        {
            auto start = std::chrono::high_resolution_clock::now();
            messages = window.TakeMessages();
            input.Update(messages);

            //Frame start
            {
                auto key = window.CreateKey();
                ge2::ImguiBeginFrame(input);
            }

            ed::Window::DrawAll();

            //Draw and display
            {
                auto key = window.CreateKey();
                gfx::ClearColour();

                ge2::ImguiEndFrame();    //Ui must be drawn last before display()
                gfx::Display(key);
            }

            using namespace std::chrono_literals;
            std::this_thread::sleep_until(start + updateLengthMicroseconds * 1us);
        }
    }
}