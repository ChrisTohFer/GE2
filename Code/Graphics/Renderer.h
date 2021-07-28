#pragma once

#include "ShaderProgram.h"
#include "Texture.h"
#include "VertexArray.h"

#include <array>

namespace ge2::gfx
{
    struct Renderer
    {
        Transform                   transform;
        ShaderProgram*              shader;
        VertexArray*                vertices;
        std::array<Texture2D*, 8u>  textures;
    };
}