#pragma once

#include "Texture.h"
#include "ShaderProgram.h"

#include "AssetManager/Loader.h"

#include <vector>

namespace ge2::gfx
{

    struct PNGLoader : public ast::Loader
    {
        bool LoadFile(std::wstring const& file) override;
        std::wstring Extension() const override;

        std::vector<Texture2D> textures;
    };
    struct JPGLoader : public ast::Loader
    {
        bool LoadFile(std::wstring const& file) override;
        std::wstring Extension() const override;

        std::vector<Texture2D> textures;
    };
    struct ShaderLoader : public ast::Loader
    {
        bool LoadFile(std::wstring const& file) override;
        std::wstring Extension() const override;

        std::vector<ShaderProgram> shaders;
    };

}