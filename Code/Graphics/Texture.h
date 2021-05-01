#pragma once

namespace ge2::gfx
{

    class Texture2D
    {
    public:
        Texture2D(const char* filename, bool hasAlphaChannel);
        ~Texture2D();

        void MakeActive(int unit = 0);

    private:
        unsigned int m_id;
        int          m_width;
        int          m_height;
        int          m_nChannels;
        int          m_success;
    };

}