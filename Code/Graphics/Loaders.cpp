#include "Loaders.h"

namespace ge2::gfx
{
    bool PNGLoader::LoadFile(std::wstring const& file)
    {
        std::string fileNarrow(file.begin(), file.end());
        textures.push_back(Texture2D(fileNarrow.c_str(), true));
        return textures.back().LoadedSuccessfully();
    }
    std::wstring PNGLoader::Extension() const
    {
        return L"png";
    }

    bool JPGLoader::LoadFile(std::wstring const& file)
    {
        std::string fileNarrow(file.begin(), file.end());
        textures.push_back(Texture2D(fileNarrow.c_str(), false));
        return textures.back().LoadedSuccessfully();
    }
    std::wstring JPGLoader::Extension() const
    {
        return L"jpg";
    }
}
