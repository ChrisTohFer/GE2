#include "Core.h"

#include "AssetManager/Assets.h"

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "SFML/Window.hpp"

#include <algorithm>

namespace
{
    bool graphicsInitialised = false;

    static glm::mat4 Convert(ge2::Matrix4x4f m)
    {
        return {
            {m.values[0][0], m.values[1][0], m.values[2][0], m.values[3][0]},
            {m.values[0][1], m.values[1][1], m.values[2][1], m.values[3][1]},
            {m.values[0][2], m.values[1][2], m.values[2][2], m.values[3][2]},
            {m.values[0][3], m.values[1][3], m.values[2][3], m.values[3][3]}
        };
    }
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
        for (int i = int(m_renderers.size()) - 1; i >= 0; --i)
        {
            delete m_renderers[i];
            m_renderers.pop_back();
        }
    }

    Renderer* Core::Find(GUID guid) const
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

    void Core::Draw(Camera const& camera, int screenX, int screenY) const
    {
        glViewport(0, 0, screenX, screenY);
        auto cameraTransform = Convert(camera.Matrix());
        cameraTransform = glm::perspective(camera.fovY, float(screenX) / float(screenY), camera.near, camera.far) * cameraTransform;

        for (auto& render : m_renderers)
        {
            if (!render->IsValid())
            {
                continue;   //Skip if we're missing shader or vertices
            }

            auto& trans = render->transform;
            auto& shaderProgram = *render->shader;
            auto& verts = *render->vertices;
            auto& textures = render->textures;
            shaderProgram.MakeActive();

            glm::mat4 transform = Convert(trans.Matrix());
            {
                unsigned int transformLoc = glGetUniformLocation(shaderProgram.Id(), "model");
                glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
            }
            {
                unsigned int cameraLoc = glGetUniformLocation(shaderProgram.Id(), "camera");
                glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(cameraTransform));
            }

            for (int i = 0; i < textures.size(); ++i)
            {
                if (textures[i])
                {
                    textures[i]->MakeActive(i);
                }
            }

            verts.Draw();
        }
    }
}