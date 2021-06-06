#pragma once

#include <set>
#include <string>

namespace ge2::assets
{
    struct Loader
    {
        virtual bool LoadFile(std::wstring const& file) = 0;
        virtual std::wstring_view Extension() const = 0;
    };

    std::string LoadTextFile(std::wstring const& file);

}