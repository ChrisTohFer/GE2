#pragma once

#include <set>
#include <string>

namespace ge2::ast
{

    class Loader
    {
    public:
        Loader(std::wstring const& extension);
        virtual bool LoadFile(std::wstring const& file) = 0;

        const std::wstring& Extension() const;
    private:
        const std::wstring m_extension;
    };

}