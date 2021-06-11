#include "ImguiHelpers.h"

#include "platform/imgui_backend/imgui_impl_opengl3.h"
#include "platform/imgui_backend/imgui_impl_ge2.h"

#include "SFML/Window.hpp"

namespace
{
    ImGuiIO*                  io;    //If null then imgui is uninitialised
}

namespace ge2
{
    using namespace plat;

    void InitialiseImgui(Window::WindowKey const& key)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplGe2_Init(key);
        ImGui_ImplOpenGL3_Init();
        io = &ImGui::GetIO();
    }
    void ShutdownImgui()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGe2_Shutdown();
        ImGui::DestroyContext();
        io = nullptr;
    }
    void ImguiBeginFrame(Input const& input)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGe2_NewFrame(input);
        ImGui::NewFrame();
    }
    void ImguiEndFrame()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

}