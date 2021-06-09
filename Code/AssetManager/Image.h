#pragma once

#include <string>

namespace ge2::assets
{
    class Image
    {
    public:
        Image(std::wstring const& filepath);
        ~Image();
        Image(Image const&) = delete;
        Image(Image&&) noexcept;
        Image& operator=(Image const&) = delete;
        Image& operator=(Image&&) noexcept;

        bool Valid() const;
        unsigned char* const Data() const;
        int Width() const;
        int Height() const;
        int Channels() const;

    private:
        unsigned char* m_data = nullptr;
        int m_width;
        int m_height;
        int m_nChannels;
    };
}