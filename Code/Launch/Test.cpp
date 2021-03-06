#include "Platform/Window.h"

#include "imgui.h"
#include "backend/imgui_impl_opengl2.h"
#include "backend/imgui_impl_win32.h"

#include "SFML/Window.hpp"
#include "SFML/OpenGL.hpp"

#include <thread>

int main()
{
    using namespace ge2;
    using namespace ge2::plat;

    Window window;
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    {
        Window::WindowKey key(window);
        ImGui_ImplWin32_Init(key.Window()->getSystemHandle());
        ImGui_ImplOpenGL2_Init();
        ImGui_ImplOpenGL2_CreateFontsTexture();
    }
    ImGuiIO& io = ImGui::GetIO();

    int updateLengthMicroseconds = 5000;
    WindowMessages messages;
    while (!messages.closeButtonPressed)
    {
        auto start = std::chrono::high_resolution_clock::now();
        messages = window.TakeMessages();

        {
            Window::WindowKey key(window);
            glClearColor(0, 0, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            ImGui_ImplWin32_NewFrame();
            ImGui_ImplOpenGL2_NewFrame();
            ImGui::NewFrame();
        }
        
        ImGui::Begin("Window");
        ImGui::Text("Testing");
        ImGui::End();
        ImGui::ShowDemoWindow();
        
        {
            Window::WindowKey key(window);
            ImGui::Render();
            ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        
            key.Window()->display();
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