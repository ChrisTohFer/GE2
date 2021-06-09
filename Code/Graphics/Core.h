#pragma once

#include "Shapes.h"

#include "Platform/Window.h"

namespace ge2::gfx
{
    class Core
    {
    public:
        Core();
        ~Core();
        Core(Core const&) = delete;
        Core(Core&&) = delete;
        Core& operator=(Core const&) = delete;
        Core& operator=(Core&&) = delete;

        void ClearColour() const;
        void Display(plat::Window::WindowKey const& key) const;

    private:
        Shapes m_shapes;
    };
}