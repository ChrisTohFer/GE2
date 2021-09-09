#pragma once

#include "ShaderProgram.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Global/Transform.h"
#include "Platform/Guid.h"

#include <array>

namespace ge2::gfx
{
    struct Renderer
    {
        ShaderProgram const*                shader;
        VertexArray const*                  vertices;
        std::array<Texture2D const*, 8u>    textures = { nullptr };

        bool HasValidShader() const;
        bool HasValidVertices() const;
        bool IsValid() const;
    };

    inline bool Renderer::HasValidShader() const
    {
        return shader && shader->CompiledWithoutError();
    }

    inline bool Renderer::HasValidVertices() const
    {
        return vertices && vertices->Valid();
    }

    //true if shader and vertices are valid, but doesn't check textures
    inline bool Renderer::IsValid() const
    {
        return HasValidShader() && HasValidVertices();
    }
}