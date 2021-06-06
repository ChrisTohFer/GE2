#pragma once

#include "Metadata.h"
#include "Platform/Guid.h"

#include <string>
#include <unordered_map>

namespace ge2::assets
{
    //Forward dec
    void AddLoader(struct LoaderBase& loader, std::wstring_view const& extension);
    //

    std::string LoadTextFile(std::wstring const& file);

    struct LoaderBase
    {
        virtual void LoadFile(std::wstring const& file, std::wstring const& filename) = 0;
    };

    template<typename LOADED_TYPE>
    struct Loader : LoaderBase
    {
        std::unordered_map<GUID, LOADED_TYPE> files;
        
        Loader(std::wstring_view const& extension);
        
        void LoadFile(std::wstring const& file, std::wstring const& filename) final;
        virtual LOADED_TYPE Load(std::wstring const& file) const = 0;

        LOADED_TYPE const* File(GUID guid);
        LOADED_TYPE const* File(std::wstring_view const& filename);
    };

    //Function definitions

    template<typename LOADED_TYPE>
    inline Loader<LOADED_TYPE>::Loader(std::wstring_view const& extension)
    {
        AddLoader(*this, extension);
    }

    template<typename LOADED_TYPE>
    inline void Loader<LOADED_TYPE>::LoadFile(std::wstring const& file, std::wstring const& filename)
    {
        files.emplace(GUIDFromFilename(filename), Load(file));
    }

    template<typename LOADED_TYPE>
    inline LOADED_TYPE const* Loader<LOADED_TYPE>::File(GUID guid)
    {
        auto it = files.find(guid);
        if (it != files.end())
        {
            return &it->second;
        }

        return nullptr;
    }

    template<typename LOADED_TYPE>
    inline LOADED_TYPE const* Loader<LOADED_TYPE>::File(std::wstring_view const& filename)
    {
        return File(GUIDFromFilename(filename));
    }

}