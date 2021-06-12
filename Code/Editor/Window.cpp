#include "Window.h"

#include "imgui.h"

namespace ge2::ed
{
    std::vector<Window*>& Windows()
    {
        static std::vector<Window*> windows;
        return windows;
    }

    void ge2::ed::Window::DrawAll()
    {
        for (auto& window : Windows())
        {
            window->Draw();
        }
    }

    Window::Window()
    {
        Windows().push_back(this);
    }

    Window::~Window()
    {
        for (auto iter = Windows().begin(); iter != Windows().end(); ++iter)
        {
            if (*iter == this)
            {
                Windows().erase(iter);
                return;
            }
        }
    }

    void Window::Draw()
    {
        if (visible)
        {
            BeginDraw();
            DrawContent();
            for (auto& m : m_modules)
            {
                m->Draw();
            }
            EndDraw();
        }
    }

    void Window::DrawContent()
    {
    }

    void Window::BeginDraw() const
    {
        ImGui::Begin(name.c_str());
    }

    void Window::EndDraw() const
    {
        ImGui::End();
    }
}