#pragma once

#include "Texture.h"
#include "ShaderProgram.h"

#include "AssetManager/Loader.h"

#include <vector>

namespace ge2::gfx
{
    struct PNGLoader : public assets::Loader<Texture2D>
    {
        PNGLoader();

        Texture2D Load(std::wstring const& file) const override;
    };
    struct JPGLoader : public assets::Loader<Texture2D>
    {
        JPGLoader();

        Texture2D Load(std::wstring const& file) const override;
    };
    struct ShaderLoader : public assets::Loader<ShaderProgram>
    {
        ShaderLoader();

        ShaderProgram Load(std::wstring const& file) const override;
    };

}