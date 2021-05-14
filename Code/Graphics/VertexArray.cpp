#include "VertexArray.h"

#include "glad/glad.h"

#include <limits>

namespace
{
    constexpr unsigned int INVALID_ID = std::numeric_limits<unsigned int>::max();
}

namespace ge2::gfx
{

    AttribPointer::AttribPointer(unsigned int location_arg, int count_arg, bool normalized_arg)
        : location(location_arg)
        , count(count_arg)
        , normalized(normalized_arg)
    {
    }

    unsigned int AttribPointer::Size() const
    {
        return count * sizeof(float);
    }

    VertexArray::VertexArray(std::vector<float> const& vertices, std::vector<AttribPointer> const& attribPointers, std::vector<unsigned int> const& elements)
    {
        glGenVertexArrays(1, &m_vertexArrayId);
        glBindVertexArray(m_vertexArrayId);

        glGenBuffers(1, &m_vertexBufferId);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        const auto stride = SetAttribPointers(attribPointers);
        
        if (elements.empty())
        {
            m_count = unsigned int(vertices.size() * sizeof(float)) / stride;
            m_elementBufferId = INVALID_ID;
            return;
        }

        m_count = unsigned int(elements.size());
        glGenBuffers(1, &m_elementBufferId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(unsigned int), elements.data(), GL_STATIC_DRAW);
    }

    void VertexArray::Draw() const
    {
        glBindVertexArray(m_vertexArrayId);
        if (m_elementBufferId == INVALID_ID)
        {
            glDrawArrays(GL_TRIANGLES, 0, m_count);
        }
        else
        {
            glDrawElements(GL_TRIANGLES, m_count, GL_UNSIGNED_INT, nullptr);
        }
    }

    unsigned int VertexArray::SetAttribPointers(std::vector<AttribPointer> const& attribPointers)
    {
        //First calculate the stride in bytes
        unsigned int stride = 0;
        for (auto const& p : attribPointers)
        {
            stride += p.Size();
        }

        //Now set the attribute pointers
        unsigned long long offset = 0;
        for (auto const& p : attribPointers)
        {
            glVertexAttribPointer(p.location, p.count, GL_FLOAT, p.normalized, stride, (void*)offset);
            glEnableVertexAttribArray(p.location);

            offset += p.Size();
        }

        return stride;
    }

}