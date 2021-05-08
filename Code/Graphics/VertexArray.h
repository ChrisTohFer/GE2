#pragma once

#include <vector>

namespace ge2::gfx
{

    struct AttribPointer
    {
        unsigned int location = 0;
        int count;
        bool normalized = false;

        AttribPointer(unsigned int location, int count, bool normalized = false);
        unsigned int Size() const;
    };

    class VertexArray
    {
    public:
        VertexArray(std::vector<float> const& vertices, std::vector<AttribPointer> const& attribPointers, std::vector<unsigned int> const& elements = {});

        void Draw() const;

    private:
        unsigned int SetAttribPointers(std::vector<AttribPointer> const& attribPointers);

        unsigned int m_vertexBufferId;
        unsigned int m_elementBufferId;
        unsigned int m_vertexArrayId;
        unsigned int m_count;           //Either number of elements in element buffer, or number of vertices if no element buffer
    };

}