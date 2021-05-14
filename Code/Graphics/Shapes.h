#pragma once
#include "VertexArray.h"

namespace ge2::gfx
{
    class Shapes
    {
    public:
        static VertexArray const& Cube();
        static VertexArray const& Sprite();

        Shapes();
    private:
        inline static Shapes* m_singleton = nullptr;
        const VertexArray m_cube;
        const VertexArray m_sprite;
    };
    
}