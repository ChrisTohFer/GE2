#pragma once

#include <set>
#include <string>

namespace ge2::ast
{
    struct Loader
    {
        virtual bool LoadFile(std::wstring const& file) = 0;
        virtual std::wstring Extension() const = 0;
    };
}