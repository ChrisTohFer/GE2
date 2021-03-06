#include "Platform/Window.h"

#include "imgui.h"
#include "backend/imgui_impl_opengl2.h"
#include "backend/imgui_impl_win32.h"

#include "SFML/Window.hpp"

#include <thread>

int main()
{
    using namespace ge2;
    using namespace ge2::plat;

    Window window;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    

    {
        Window::WindowKey key(window);
        ImGui_ImplWin32_Init(key.Window()->getSystemHandle());
        ImGui_ImplOpenGL2_Init();
        ImGui_ImplOpenGL2_CreateFontsTexture();
    }

    int updateLengthMicroseconds = 5000;
    WindowMessages messages;
    while (!messages.closeButtonPressed)
    {
        auto start = std::chrono::high_resolution_clock::now();
        messages = window.TakeMessages();

        {
            Window::WindowKey key(window);
            ImGui_ImplWin32_NewFrame();
            ImGui_ImplOpenGL2_NewFrame();
        }
        ImGui::NewFrame();

        ImGui::Begin("Window");
        ImGui::Text("Testing");
        ImGui::End();

        ImGui::Render();
        {
            Window::WindowKey key(window);
            auto drawData = ImGui::GetDrawData();
            if (drawData)
            {
                ImGui_ImplOpenGL2_RenderDrawData(drawData);
            }
        }

        using namespace std::chrono_literals;
        std::this_thread::sleep_until(start + updateLengthMicroseconds * 1us);
    }

    {
        Window::WindowKey key(window);
        ImGui_ImplOpenGL2_DestroyFontsTexture();
        ImGui_ImplOpenGL2_Shutdown();
        ImGui_ImplWin32_Shutdown();
    }

    ImGui::DestroyContext();
    window.Close();
    
    return 0;
}