#pragma once

#include <memory>

namespace ge2
{

    class Buffer
    {
    public:
        Buffer() {};
        Buffer(int size)
            : m_size(size)
            , m_buffer(new std::byte[size])
        {};
        Buffer(Buffer const& other)
            : m_size(other.m_size)
            , m_buffer(new std::byte[m_size])
        {
            memcpy(m_buffer, other.m_buffer, m_size);
        }
        Buffer(Buffer&& other) noexcept
            : m_size(other.m_size)
            , m_buffer(other.m_buffer)
        {
            other.m_buffer = nullptr;
        }
        ~Buffer()
        {
            delete m_buffer;
        }
        Buffer& operator=(Buffer const& other)
        {
            m_size = other.m_size;
            m_buffer = new std::byte[m_size];
            memcpy(m_buffer, other.m_buffer, m_size);
            return *this;
        }
        Buffer& operator=(Buffer&& other) noexcept
        {
            m_size = other.m_size;
            m_buffer = other.m_buffer;
            other.m_buffer = nullptr;
            return *this;
        }

        std::byte* Data() const
        {
            return m_buffer;
        }
        int Size() const
        {
            return m_size;
        }

    private:
        std::byte* m_buffer = nullptr;
        int m_size = 0;
    };

}