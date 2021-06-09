#include "Texture.h"

#include "AssetManager/CommonLoaders.h"
#include "AssetManager/Metadata.h"
#include "glad/glad.h"

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

    Texture2D::Texture2D(assets::Image const& image)
        : m_valid(false)
        , m_width(image.Width())
        , m_height(image.Height())
        , m_nChannels(image.Channels())
        , m_id(0)
    {
        if (!image.Valid())
        {
            return;
        }
        m_valid = true;

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //Assuming 4 channels means there's a transparency channel, not sure if this is always true
        if (image.Channels() == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.Data());
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.Data());
        }

        glGenerateMipmap(GL_TEXTURE_2D);
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

    void Texture2D::MakeActive(int unit) const
    {
        _ASSERT(unit >= 0 && unit < 16);    //Texture unit might be out of bounds
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }

    bool Texture2D::Valid() const
    {
        return m_valid;
    }

    std::unordered_map<GUID, Texture2D> g_textures;

    Texture2D const* TextureFromGuid(GUID guid)
    {
        if (g_textures.size() == 0)
        {
            auto const& loader = assets::ImageLoader::Instance();
            for (auto const& image : loader.Files())
            {
                g_textures.emplace(image.first, Texture2D(image.second));
            }
        }

        auto it = g_textures.find(guid);
        if (it != g_textures.end())
        {
            return &it->second;
        }

        return nullptr;
    }

    Texture2D const* TextureFromFilename(std::wstring_view const& filename)
    {
        return TextureFromGuid(assets::GUIDFromFilename(filename));
    }
}