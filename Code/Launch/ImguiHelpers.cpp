#include "ImguiHelpers.h"

#include "backend/imgui_impl_opengl2.h"
#include "backend/imgui_impl_win32.h"

#include "SFML/Window.hpp"

namespace
{
    ImGuiIO* io;    //If null then imgui is uninitialised
}

namespace ge2
{
    using namespace plat;

    void HandleInputEvents(Input const& input)
    {
        //Mouse events
        auto position = input.MousePosition();
        auto delta = input.MouseDelta();
        io->MouseDelta = { delta.x, delta.y };
        for (int i = 0; i < 5; ++i) //Mouse left, right, middle, x1, x2
        {
            auto button = Input::Button(i + Input::Button::MouseLeft);

            io->MouseClicked[i] = input.ButtonDown(button);
            io->MouseClickedPos[i] = { position.x, position.y };
            io->MouseDown[i] = input.ButtonHeld(button);
        }
    }

    void InitialiseImgui(Window::WindowKey const& key)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplWin32_Init(key.Window()->getSystemHandle());
        ImGui_ImplOpenGL2_Init();
        ImGui_ImplOpenGL2_CreateFontsTexture();
        io = &ImGui::GetIO();
    }
    void ShutdownImgui(Window::WindowKey const&)                                          //WindowKey parameter unused, but must be provided to prove we have window access
    {
        ImGui_ImplOpenGL2_DestroyFontsTexture();
        ImGui_ImplOpenGL2_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        io = nullptr;
    }
    void ImguiBeginFrame(Window::WindowKey const&, Input const& input)  //WindowKey parameter unused, but must be provided to prove we have window access
    {
        ImGui_ImplWin32_NewFrame();
        ImGui_ImplOpenGL2_NewFrame();
        ImGui::NewFrame();

        HandleInputEvents(input);
    }
    void ImguiEndFrame(Window::WindowKey const&)                                          //WindowKey parameter unused, but must be provided to prove we have window access
    {
        ImGui::Render();
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    }

}