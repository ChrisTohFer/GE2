#include "Image.h"

#include "stb_image.h"

#include <iostream>

ge2::assets::Image::Image(std::wstring const& filepath)
{
    std::string fileNarrow(filepath.begin(), filepath.end());
    stbi_set_flip_vertically_on_load(1);
    m_data = stbi_load(fileNarrow.c_str(), &m_width, &m_height, &m_nChannels, 0);

    if (!m_data)
    {
        stbi_image_free(m_data);
        std::cout << "Failed to load image:\t" << fileNarrow << "\n";
    }
}

ge2::assets::Image::~Image()
{
    stbi_image_free(m_data);
}

ge2::assets::Image::Image(Image&& other) noexcept
    : m_data(other.m_data)
    , m_width(other.m_width)
    , m_height(other.m_height)
    , m_nChannels(other.m_nChannels)
{
    other.m_data = nullptr;
}

ge2::assets::Image& ge2::assets::Image::operator=(Image&& other) noexcept
{
    m_data = other.m_data;
    m_width = other.m_width;
    m_height = other.m_height;
    m_nChannels = other.m_nChannels;

    other.m_data = nullptr;

    return *this;
}

bool ge2::assets::Image::Valid() const
{
    return m_data != nullptr;
}

unsigned char* const ge2::assets::Image::Data() const
{
    return m_data;
}

int ge2::assets::Image::Width() const
{
    return m_width;
}

int ge2::assets::Image::Height() const
{
    return m_height;
}

int ge2::assets::Image::Channels() const
{
    return m_nChannels;
}
