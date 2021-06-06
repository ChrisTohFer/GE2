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
        assets::AddLoader(m_jpgLoader);
        assets::AddLoader(m_pngLoader);
        assets::AddLoader(m_shaderLoader);
    }

    Core::~Core()
    {
        g_core = nullptr;

        assets::RemoveLoader(m_jpgLoader);
        assets::RemoveLoader(m_pngLoader);
        assets::RemoveLoader(m_shaderLoader);
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