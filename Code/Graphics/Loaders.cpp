#include "Loaders.h"
#include "AssetManager/Assets.h"

#include <fstream>

namespace ge2::gfx
{
    PNGLoader::PNGLoader()
    {
        assets::AddLoader(*this);
    }

    PNGLoader::~PNGLoader()
    {
        assets::RemoveLoader(*this);
    }

    bool PNGLoader::LoadFile(std::wstring const& file)
    {
        std::string fileNarrow(file.begin(), file.end());
        textures.push_back(Texture2D(fileNarrow.c_str(), true));
        return textures.back().Valid();
    }
    std::wstring_view PNGLoader::Extension() const
    {
        return std::wstring_view(L"png");
    }

    JPGLoader::JPGLoader()
    {
        assets::AddLoader(*this);
    }

    JPGLoader::~JPGLoader()
    {
        assets::RemoveLoader(*this);
    }

    bool JPGLoader::LoadFile(std::wstring const& file)
    {
        std::string fileNarrow(file.begin(), file.end());
        textures.push_back(Texture2D(fileNarrow.c_str(), false));
        return textures.back().Valid();
    }
    std::wstring_view JPGLoader::Extension() const
    {
        return std::wstring_view(L"jpg");
    }

    ShaderLoader::ShaderLoader()
    {
        assets::AddLoader(*this);
    }

    ShaderLoader::~ShaderLoader()
    {
        assets::RemoveLoader(*this);
    }

    bool ShaderLoader::LoadFile(std::wstring const& vertexFileName)
    {
        //assume that every .vert shader has a matching .frag shader
        std::wstring fragFileName = vertexFileName.substr(0, vertexFileName.find_last_of(L"vert")) + L"frag";

        std::string vertexSource = assets::LoadTextFile(vertexFileName);
        std::string fragSource = assets::LoadTextFile(fragFileName);
        shaders.push_back(ShaderProgram(vertexSource.data(), fragSource.data()));

        return shaders.back().CompiledWithoutError();
    }
    std::wstring_view ShaderLoader::Extension() const
    {
        return std::wstring_view(L"vert");
    }

}
