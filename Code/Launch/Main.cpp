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

    Camera camera;
    camera.position = Vector3f{ 0.f, 0.f, -0.1f };
    camera.rotation = Quaternion::Identity();

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

        Vector3f deltaMove = Vector3f::Zero();
        Vector3f deltaEuler = Vector3f::Zero();
        if (input.ButtonHeld(Input::Button::W)) deltaMove.z -= 5.f / 60.f;
        if (input.ButtonHeld(Input::Button::S)) deltaMove.z += 5.f / 60.f;
        if (input.ButtonHeld(Input::Button::A)) deltaMove.x -= 5.f / 60.f;
        if (input.ButtonHeld(Input::Button::D)) deltaMove.x += 5.f / 60.f;
        if (input.ButtonHeld(Input::Button::Space)) deltaMove.y += 5.f / 60.f;
        if (input.ButtonHeld(Input::Button::LControl)) deltaMove.y -= 5.f / 60.f;
        if (input.ButtonHeld(Input::Button::Up)) deltaEuler.x -= 5.f / 60.f;
        if (input.ButtonHeld(Input::Button::Down)) deltaEuler.x += 5.f / 60.f;
        if (input.ButtonHeld(Input::Button::Left)) deltaEuler.y -= 5.f / 60.f;
        if (input.ButtonHeld(Input::Button::Right)) deltaEuler.y += 5.f / 60.f;
        if (input.ButtonHeld(Input::Button::Q)) deltaEuler.z -= 5.f / 60.f;
        if (input.ButtonHeld(Input::Button::E)) deltaEuler.z += 5.f / 60.f;

        camera.position += camera.rotation.Inverse() * deltaMove;
        //camera.rotation.Euler(camera.rotation.Euler() + deltaEuler);
        camera.LookAt(Vector3f::Zero());

        ImGui::Begin("Camera euler");
        ImGui::Text("%f", camera.rotation.Euler().x);
        ImGui::Text("%f", camera.rotation.Euler().y);
        ImGui::Text("%f", camera.rotation.Euler().z);
        ImGui::End();
        ImGui::Begin("Camera position");
        ImGui::Text("%f", camera.position.x);
        ImGui::Text("%f", camera.position.y);
        ImGui::Text("%f", camera.position.z);
        ImGui::End();
        ImGui::Begin("Camera quaternion");
        ImGui::Text("%f", camera.rotation.w);
        ImGui::Text("%f", camera.rotation.v.x);
        ImGui::Text("%f", camera.rotation.v.y);
        ImGui::Text("%f", camera.rotation.v.z);
        ImGui::End();

        gfx::UpdateCamera(camera);
        
        //Draw and display
        {
            auto key = window.CreateKey();
            gfx::ClearColour();

            gfx::DrawTriangle(Vector3f{0.f, 0.f, 2.f}, Vector3f{ 0.f,0.f,1.f });
            gfx::DrawTriangle(Vector3f{0.f, 0.f,-2.f}, Vector3f{ 0.f,0.f,0.f });
            gfx::DrawTriangle(Vector3f{2.f, 0.f, 0.f}, Vector3f{ 0.f,0.f,0.f });
            gfx::DrawTriangle(Vector3f{-2.f,0.f, 0.f}, Vector3f{ 0.f,0.f,0.f });
            gfx::DrawTriangle(Vector3f{0.f, 2.f, 0.f}, Vector3f{ 0.f,0.f,0.f });
            gfx::DrawTriangle(Vector3f{0.f,-2.f, 0.f}, Vector3f{ 0.f,0.f,0.f });

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