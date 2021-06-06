#include "Loaders.h"
#include "AssetManager/Assets.h"

#include <fstream>

namespace ge2::gfx
{
    PNGLoader::PNGLoader()
        : Loader(L".png")
    {
    }

    Texture2D PNGLoader::Load(std::wstring const& file) const
    {
        std::string fileNarrow(file.begin(), file.end());
        return Texture2D(fileNarrow.c_str(), true);
    }

    JPGLoader::JPGLoader()
        : Loader(L".jpg")
    {
    }

    Texture2D JPGLoader::Load(std::wstring const& file) const
    {
        std::string fileNarrow(file.begin(), file.end());
        return Texture2D(fileNarrow.c_str(), false);
    }

    ShaderLoader::ShaderLoader()
        : Loader(L".vert")
    {
    }

    ShaderProgram ShaderLoader::Load(std::wstring const& vertexFileName) const
    {
        //assume that every .vert shader has a matching .frag shader
        std::wstring fragFileName = vertexFileName.substr(0, vertexFileName.find_last_of(L"vert")) + L"frag";

        std::string vertexSource = assets::LoadTextFile(vertexFileName);
        std::string fragSource = assets::LoadTextFile(fragFileName);

        return ShaderProgram(vertexSource.data(), fragSource.data());
    }

}
