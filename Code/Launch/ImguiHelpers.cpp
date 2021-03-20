#include "ImguiHelpers.h"

#include "platform/imgui_backend/imgui_impl_opengl3.h"
#include "platform/imgui_backend/imgui_impl_ge2.h"

#include "SFML/Window.hpp"

namespace
{
    ImGuiIO*                  io;    //If null then imgui is uninitialised
    const ge2::plat::Window*  window;
}

namespace ge2
{
    void InitialiseImgui(Window const& windowArg)
    {
        auto key = windowArg.CreateKey();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplGe2_Init(key);
        ImGui_ImplOpenGL3_Init();
        io = &ImGui::GetIO();
        window = &windowArg;
    }
    void ShutdownImgui()
    {
        auto key = window->CreateKey();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGe2_Shutdown();
        ImGui::DestroyContext();
        io = nullptr;
        window = nullptr;
    }
    void ImguiBeginFrame(Input const& input)
    {
        auto key = window->CreateKey();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGe2_NewFrame(input);
        ImGui::NewFrame();
    }
    void ImguiEndFrame()
    {
        auto key = window->CreateKey();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

}