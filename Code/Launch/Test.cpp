#include "ImguiHelpers.h"

#include "Platform/Window.h"
#include "Platform/Input.h"

#include "imgui.h"
#include "SFML/Window.hpp"
#include "SFML/OpenGL.hpp"

#include <thread>

int main()
{
    using namespace ge2;
    using namespace ge2::plat;

    Window window;
    Input input;

    ge2::InitialiseImgui(window.CreateKey());

    int updateLengthMicroseconds = 5000;
    WindowMessages messages;
    while (!messages.closeButtonPressed)
    {
        auto start = std::chrono::high_resolution_clock::now();
        messages = window.TakeMessages();
        input.Update(messages);

        {
            auto key = window.CreateKey();
            glClearColor(0, 0, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            ge2::ImguiBeginFrame(key, input);
        }

        

        ImGui::Begin("Window");
        ImGui::Text("%f", input.MouseWheelDelta());
        ImGui::End();
        ImGui::ShowDemoWindow();
        
        {
            auto key = window.CreateKey();
            ge2::ImguiEndFrame(key);
            key.Window()->display();
        }

        using namespace std::chrono_literals;
        std::this_thread::sleep_until(start + updateLengthMicroseconds * 1us);
    }

    ge2::ShutdownImgui(window.CreateKey());
    window.Close();
    
    return 0;
}