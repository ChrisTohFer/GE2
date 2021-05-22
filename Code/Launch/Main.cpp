#include "ImguiHelpers.h"

#include "AssetManager/Assets.h"
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
        ast::LoadAllAssets();
    }

    gfx::Camera camera;
    camera.position = Vector3f{ 0.f, 0.f, 2.f };

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

        if (input.ButtonHeld(Input::Button::W)) camera.positionDelta.z -= 5.f / 60.f;
        if (input.ButtonHeld(Input::Button::S)) camera.positionDelta.z += 5.f / 60.f;
        if (input.ButtonHeld(Input::Button::A)) camera.positionDelta.x -= 5.f / 60.f;
        if (input.ButtonHeld(Input::Button::D)) camera.positionDelta.x += 5.f / 60.f;
        if (input.ButtonHeld(Input::Button::Up)) camera.rotation.x -= 5.f / 60.f;
        if (input.ButtonHeld(Input::Button::Down)) camera.rotation.x += 5.f / 60.f;
        if (input.ButtonHeld(Input::Button::Left)) camera.rotation.y -= 5.f / 60.f;
        if (input.ButtonHeld(Input::Button::Right)) camera.rotation.y += 5.f / 60.f;
        gfx::UpdateCamera(camera);
        
        //Draw and display
        {
            auto key = window.CreateKey();
            gfx::ClearColour();

            gfx::DrawTriangle(Vector3f{1.f,1.f,0.f}, Vector3f{ 0.f,0.f,1.f });
            gfx::DrawTriangle(Vector3f{-1.f,-1.f,0.f}, Vector3f{ 0.f,0.f,0.f });
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