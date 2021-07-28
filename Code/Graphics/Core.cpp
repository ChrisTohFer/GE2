#include "Core.h"

#include "AssetManager/Assets.h"

#include "glad/glad.h"
#include "SFML/Window.hpp"

#include <algorithm>

namespace
{
    bool graphicsInitialised = false;
}

namespace ge2::gfx
{
    void Initialise()
    {
        if (graphicsInitialised)
        {
            return;
        }
        graphicsInitialised = true;

        gladLoadGLLoader((GLADloadproc)sf::Context::getFunction);
        glEnable(GL_DEPTH_TEST);
    }

    void ClearColour()
    {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Display(plat::Window::WindowKey const& key)
    {
        key.Window()->display();
    }

    Core::~Core()
    {
        for (int i = m_renderers.size() - 1; i >= 0; --i)
        {
            delete m_renderers[i];
            m_renderers.pop_back();
        }
    }

    Renderer* Core::Find(GUID guid)
    {
        for (auto& render : m_renderers)
        {
            if (render->guid == guid)
            {
                return render;
            }
        }

        return nullptr;
    }

    void Core::Add(Renderer const& renderer)
    {
#ifdef _DEBUG
        for (auto& render : m_renderers)
        {
            _ASSERT(render->guid != renderer.guid); //Tried to add renderer with pre-existing guid
        }
#endif

        m_renderers.push_back(new Renderer(renderer));
    }

    void Core::Remove(GUID guid)
    {
        auto element = std::find_if(m_renderers.begin(), m_renderers.end(), [&](auto& elem)
            {
                return elem->guid == guid;
            });

        if (element != m_renderers.end())
        {
            std::swap(m_renderers.back(), *element);
            m_renderers.pop_back();
        }
        else
        {
            _ASSERT(false); //Tried to remove renderer that doesn't exist
        }
    }
}