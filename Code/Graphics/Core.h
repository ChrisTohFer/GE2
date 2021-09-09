#pragma once

#include "Camera.h"
#include "Renderer.h"
#include "Platform/Guid.h"
#include "Platform/Window.h"

#include <vector>

namespace ge2::gfx
{
    void Initialise();
    void ClearColour();
    void Display(plat::Window::WindowKey const&);

    class Core
    {
    public:
        Core() = default;
        Core(Core const&) = delete;
        Core(Core&&) = delete;
        Core& operator=(Core const&) = delete;
        Core& operator=(Core&&) = delete;

        ~Core();

        Renderer* Find(GUID) const;
        void Add(Renderer const&);
        void Remove(GUID);

        void Draw(Camera const&, int screenX, int screenY) const;

    private:
        std::vector<Renderer*> m_renderers;
    };
}