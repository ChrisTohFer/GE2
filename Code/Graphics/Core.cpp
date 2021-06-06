#include "Core.h"

#include "AssetManager/Assets.h"

#include "glad/glad.h"
#include "SFML/Window.hpp"

namespace
{
    ge2::gfx::Core* g_core = nullptr;
}

namespace ge2::gfx
{
    Core::Core()
    {
        _ASSERT(g_core == nullptr);
        g_core = this;

        gladLoadGLLoader((GLADloadproc)sf::Context::getFunction);
        glEnable(GL_DEPTH_TEST);
        m_shapes.Initialise();
    }

    Core::~Core()
    {
        g_core = nullptr;
    }

    void Core::ClearColour() const
    {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Core::Display(plat::Window::WindowKey const& key) const
    {
        key.Window()->display();
    }
}