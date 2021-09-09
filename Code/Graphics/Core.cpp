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

    struct RendererInternal
    {
        ge2::Transform transform;
        ge2::gfx::Renderer renderer;
    };

    std::vector<RendererInternal> renderers;

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

    void AddRenderer(Transform const& transform, Renderer const& renderer)
    {
        renderers.push_back({ transform, renderer });
    }

    void Draw(Camera const& camera, int screenX, int screenY)
    {
        glViewport(0, 0, screenX, screenY);
        auto cameraTransform = Convert(camera.Matrix());
        cameraTransform = glm::perspective(camera.fovY, float(screenX) / float(screenY), camera.near, camera.far) * cameraTransform;

        for (auto& renderInternal : renderers)
        {
            Renderer& render = renderInternal.renderer;
            if (!render.IsValid())
            {
                continue;   //Skip if we're missing shader or vertices
            }

            auto& trans = renderInternal.transform;
            auto& shaderProgram = *render.shader;
            auto& verts = *render.vertices;
            auto& textures = render.textures;
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

        renderers.clear();
    }
}