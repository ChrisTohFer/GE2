#pragma once
#include "VertexArray.h"

namespace ge2::gfx
{
    class Shapes
    {
    public:
        void Initialise();

        VertexArray const& Cube() const;
        VertexArray const& Sprite() const;

    private:
        VertexArray m_cube;
        VertexArray m_sprite;
    };
    
}