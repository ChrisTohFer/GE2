#pragma once

#include "Texture.h"
#include "ShaderProgram.h"

#include "AssetManager/Loader.h"

#include <vector>

namespace ge2::gfx
{
    struct PNGLoader : public assets::Loader
    {
        PNGLoader();
        ~PNGLoader();

        bool LoadFile(std::wstring const& file) override;
        std::wstring_view Extension() const override;

        std::vector<Texture2D> textures;
    };
    struct JPGLoader : public assets::Loader
    {
        JPGLoader();
        ~JPGLoader();

        bool LoadFile(std::wstring const& file) override;
        std::wstring_view Extension() const override;

        std::vector<Texture2D> textures;
    };
    struct ShaderLoader : public assets::Loader
    {
        ShaderLoader();
        ~ShaderLoader();

        bool LoadFile(std::wstring const& file) override;
        std::wstring_view Extension() const override;

        std::vector<ShaderProgram> shaders;
    };

}