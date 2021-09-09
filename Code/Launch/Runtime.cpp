#include "Scene.h"
#include "Graphics/Core.h"
#include "Graphics/Shapes.h"
#include "Platform/ImguiHelpers.h"
#include "Platform/Window.h"

#include "imgui.h"

#include <chrono>
#include <thread>

namespace ge2
{
    void AddEntity(Scene& scene, Transform transform, bool crate)
    {
        Entity entity;
        entity.transform = transform;

        entity.renderer.shader = gfx::ShaderLoader::Instance().File(L"diffuse.shad");
        entity.renderer.textures[0] = gfx::TextureFromFilename(crate ? L"container.jpg" : L"awesomeface.png");
        entity.renderer.vertices = crate ? &gfx::Cube() : &gfx::Sprite();

        entity.renderer.shader->SetUniform("tex", 0);

        scene.entities.push_back(std::move(entity));
    }

    void RuntimeLoop(plat::Window& window)
    {
        using namespace ge2;
        using namespace ge2::plat;

        Input input;
        Camera camera;
        Transform& camTrans = camera.transform;
        camTrans.position = Vector3f{ 0.f, 0.f, -0.1f };
        camTrans.rotation = Quaternion::Identity();

        //Transform box;
        //Vector3f euler = Vector3f::Zero();

        Scene scene;
        {
            auto key = window.CreateKey();
            AddEntity(scene, Transform{ Vector3f{ 0.f, 0.f, 2.f }}, true);
            AddEntity(scene, Transform{ Vector3f{ 0.f, 0.f,-2.f }}, true);
            AddEntity(scene, Transform{ Vector3f{ 2.f, 0.f, 0.f }}, false);
            AddEntity(scene, Transform{ Vector3f{ -2.f,0.f, 0.f }}, true);
            AddEntity(scene, Transform{ Vector3f{ 0.f, 2.f, 0.f }}, true);
            AddEntity(scene, Transform{ Vector3f{ 0.f,-2.f, 0.f }}, true);
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
                ge2::ImguiBeginFrame(input);
            }

            //Misc

            Vector3f deltaMove = Vector3f::Zero();
            Vector3f deltaEuler = Vector3f::Zero();
            if (input.ButtonHeld(Input::Button::W)) deltaMove.z += 5.f / 60.f;
            if (input.ButtonHeld(Input::Button::S)) deltaMove.z -= 5.f / 60.f;
            if (input.ButtonHeld(Input::Button::A)) deltaMove.x += 5.f / 60.f;
            if (input.ButtonHeld(Input::Button::D)) deltaMove.x -= 5.f / 60.f;
            if (input.ButtonHeld(Input::Button::Space)) deltaMove.y += 5.f / 60.f;
            if (input.ButtonHeld(Input::Button::LControl)) deltaMove.y -= 5.f / 60.f;
            if (input.ButtonHeld(Input::Button::Up)) deltaEuler.x -= 5.f / 60.f;
            if (input.ButtonHeld(Input::Button::Down)) deltaEuler.x += 5.f / 60.f;
            if (input.ButtonHeld(Input::Button::Left)) deltaEuler.y += 5.f / 60.f;
            if (input.ButtonHeld(Input::Button::Right)) deltaEuler.y -= 5.f / 60.f;
            if (input.ButtonHeld(Input::Button::Q)) deltaEuler.z -= 5.f / 60.f;
            if (input.ButtonHeld(Input::Button::E)) deltaEuler.z += 5.f / 60.f;

            camTrans.position += camTrans.rotation * deltaMove;
            camTrans.rotation.Euler(camTrans.rotation.Euler() + deltaEuler);
            //camera.LookAt(Vector3f::Zero());

            //ImGui::Begin("BoxControl");
            //ImGui::SliderFloat3("Position", (float*)&box.position, -2.f, 2.f);
            //ImGui::SliderFloat3("Scale", (float*)&box.scale, -1.f, 1.f);
            //ImGui::SliderFloat3("Rotation", (float*)&euler, -PI, PI);
            //ImGui::End();
            //box.rotation.Euler(euler);

            //static float phase = 0.f;
            //phase += 0.015f;
            //box.rotation.NLerpTowards(Quaternion::Identity(), sinf(phase));
            //box.rotation.LookDirection(-box.position);

            //Draw and display
            {
                auto key = window.CreateKey();
                gfx::ClearColour();
                scene.Draw(camera, messages.width, messages.height);

                ge2::ImguiEndFrame();    //Ui must be drawn last before display()
                gfx::Display(key);
            }

            using namespace std::chrono_literals;
            std::this_thread::sleep_until(start + updateLengthMicroseconds * 1us);
        }
    }
}