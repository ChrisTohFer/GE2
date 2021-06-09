#include "CommonLoaders.h"

#include <fstream>

namespace ge2::assets
{
    //TextLoader

    TextLoader TextLoader::g_loader;

    TextLoader::TextLoader()
        : Loader({ L".txt", L".vert", L".frag" })
    {
    }

    TextLoader const& TextLoader::Instance()
    {
        return g_loader;
    }

    std::string TextLoader::Load(std::wstring const& filepath) const
    {
        //Get file size
        std::ifstream file(filepath);
        const auto begin = file.tellg();
        file.seekg(0, std::ios::end);
        const auto end = file.tellg();
        const auto fsize = (end - begin);

        //Read from beginning
        file.seekg(begin);

        std::string content;
        content.resize(fsize, '\0');
        file.read(content.data(), fsize);

        return content;
    }

    //ImageLoader

    ImageLoader ImageLoader::g_loader;

    ImageLoader::ImageLoader()
        : Loader({ L".png", L".jpg" })
    {
    }

    ImageLoader const& ImageLoader::Instance()
    {
        return g_loader;
    }

    Image ImageLoader::Load(std::wstring const& file) const
    {
        return Image(file);
    }
}
