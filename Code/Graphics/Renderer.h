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
        GUID                        guid;
        Transform                   transform;
        ShaderProgram*              shader;
        VertexArray*                vertices;
        std::array<Texture2D*, 8u>  textures;
    };
}