#include "ImguiHelpers.h"

#include "Platform/Input.h"
#include "Platform/Misc.h"
#include "Platform/Window.h"
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

    {
        auto key = window.CreateKey();
        gfx::Init();
        ge2::InitialiseImgui(key);
        gfx::LoadTriangle();
        gfx::LoadShaderProgram();
        gfx::LoadTexture();
    }

    int updateLengthMicroseconds = 1000000 / 60;
    WindowMessages messages;
    while (!messages.closeButtonPressed)
    {
        auto start = std::chrono::high_resolution_clock::now();
        messages = window.TakeMessages();
        input.Update(messages);

        //Frame start
        {
            auto key = window.CreateKey();
            gfx::Update(messages.width, messages.height);
            ge2::ImguiBeginFrame(input);
        }

        //Misc
        ImGui::ShowDemoWindow();
        
        //Draw and display
        {
            auto key = window.CreateKey();
            gfx::ClearColour();
            gfx::DrawTriangle();
            ge2::ImguiEndFrame();    //Ui must be drawn last before display()
            gfx::Display(key);
        }

        using namespace std::chrono_literals;
        std::this_thread::sleep_until(start + updateLengthMicroseconds * 1us);
    }

    {
        auto key = window.CreateKey();
        ge2::ShutdownImgui();
    }
    window.Close();
    
    return 0;
}