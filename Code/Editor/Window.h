#pragma once

#include "Module.h"

#include <memory>
#include <string_view>
#include <vector>

namespace ge2::ed
{
    using WindowId = unsigned int;

    class Window
    {
    public:
        static void DrawAll();

        Window();
        virtual ~Window();
        Window(Window const&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window const&) = delete;
        Window& operator=(Window&&) = delete;

        void Draw();
        virtual void DrawContent(); //Override to provide window content

        bool visible;
        std::string name;

    protected:
        std::vector<std::unique_ptr<Module>> m_modules;

    private:
        void BeginDraw() const;
        void EndDraw() const;
    };
}