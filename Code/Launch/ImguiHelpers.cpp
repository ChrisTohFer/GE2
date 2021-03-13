#include "ImguiHelpers.h"

#include "platform/imgui_backend/imgui_impl_opengl2.h"
#include "platform/imgui_backend/imgui_impl_ge2.h"

#include "SFML/Window.hpp"

namespace
{
    ImGuiIO* io;    //If null then imgui is uninitialised
}

namespace ge2
{
    void InitialiseImgui(Window::WindowKey const& key)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplGe2_Init(key);
        ImGui_ImplOpenGL2_Init();
        io = &ImGui::GetIO();
    }
    void ShutdownImgui(Window::WindowKey const&)                                          //WindowKey parameter unused, but must be provided to prove we have window access
    {
        ImGui_ImplOpenGL2_Shutdown();
        ImGui_ImplGe2_Shutdown();
        ImGui::DestroyContext();
        io = nullptr;
    }
    void ImguiBeginFrame(Window::WindowKey const&, Input const& input)  //WindowKey parameter unused, but must be provided to prove we have window access
    {
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGe2_NewFrame(input);
        ImGui::NewFrame();
    }
    void ImguiEndFrame(Window::WindowKey const&)                                          //WindowKey parameter unused, but must be provided to prove we have window access
    {
        ImGui::Render();
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    }

}