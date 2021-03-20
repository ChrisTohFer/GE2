#include "ImguiHelpers.h"

#include "Platform/Window.h"
#include "Platform/Input.h"
#include "Graphics/Temp.h"

#include "imgui.h"
#include "glad/glad.h"
#include "SFML/Window.hpp"

#include <thread>

int main()
{
    using namespace ge2;
    using namespace ge2::plat;

    Window window;
    Input input;

    gfx::Init(window);
    ge2::InitialiseImgui(window);

    int updateLengthMicroseconds = 5000;
    WindowMessages messages;
    while (!messages.closeButtonPressed)
    {
        auto start = std::chrono::high_resolution_clock::now();
        messages = window.TakeMessages();
        input.Update(messages);

        ge2::ImguiBeginFrame(input);
        ImGui::ShowDemoWindow();
        
        gfx::ClearColour();
        ge2::ImguiEndFrame();    //Ui must be drawn last before display()
        gfx::Display();

        using namespace std::chrono_literals;
        std::this_thread::sleep_until(start + updateLengthMicroseconds * 1us);
    }

    ge2::ShutdownImgui();
    gfx::Shutdown();
    window.Close();
    
    return 0;
}