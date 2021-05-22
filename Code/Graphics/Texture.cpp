#include "Texture.h"

#include "glad/glad.h"
#include "stb_image.h"

#include <iostream>

namespace ge2::gfx
{
    Texture2D::Texture2D()
        : m_id(0)
        , m_width(0)
        , m_height(0)
        , m_nChannels(0)
        , m_valid(false)
    {
    }

    Texture2D::Texture2D(const char* filename, bool hasAlphaChannel)
    {
        stbi_set_flip_vertically_on_load(1);
        unsigned char* data = stbi_load(filename, &m_width, &m_height, &m_nChannels, 0);

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (data)
        {
            m_valid = true;

            if (hasAlphaChannel)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            }
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            m_valid = false;

            std::cout << "ERROR::TEXTURE::LOAD_FAILED\n" << stbi_failure_reason() << std::endl;
        }

        stbi_image_free(data);
    }

    Texture2D::Texture2D(Texture2D&& other) noexcept
        : m_id(other.m_id)
        , m_width(other.m_width)
        , m_height(other.m_height)
        , m_nChannels(other.m_nChannels)
        , m_valid(other.m_valid)
    {
        other.m_id = 0;
        other.m_valid = false;
    }

    Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
    {
        if (m_id != 0)
        {
            glDeleteTextures(1, &m_id);
        }

        m_id = other.m_id;
        m_width = other.m_width;
        m_height = other.m_height;
        m_nChannels = other.m_nChannels;
        m_valid = other.m_valid;

        other.m_id = 0;

        return *this;
    }

    Texture2D::~Texture2D()
    {
        if (m_id != 0)
        {
            glDeleteTextures(1, &m_id);
        }
    }

    void Texture2D::MakeActive(int unit)
    {
        _ASSERT(unit >= 0 && unit < 16);    //Texture unit might be out of bounds
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }

    bool Texture2D::Valid() const
    {
        return m_valid;
    }

}