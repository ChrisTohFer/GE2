#pragma once

#include "Image.h"
#include "Loader.h"

namespace ge2::assets
{
    class TextLoader : public Loader<std::string, 3>
    {
    public:
        TextLoader();

        static TextLoader const& Instance();

    private:
        std::string Load(std::wstring const& file) const override;

        static TextLoader g_loader;
    };

    class ImageLoader : public Loader<Image, 2>
    {
    public:
        ImageLoader();

        static ImageLoader const& Instance();

    private:
        Image Load(std::wstring const& file) const override;

        static ImageLoader g_loader;
    };
}