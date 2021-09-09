#pragma once

#include "Metadata.h"
#include "Platform/Guid.h"

#include <array>
#include <string>
#include <unordered_map>

namespace ge2::assets
{
    //Forward dec
    void AddLoader(class LoaderBase& loader, std::wstring_view const& extension);
    void LoadFile(Metadata& metadata);
    //

    class LoaderBase
    {
    public:
        virtual void LoadFile(std::wstring const& file, std::wstring const& filename) = 0;
    };

    template<typename LOADED_TYPE, int NUM_EXTENSIONS>
    class Loader : LoaderBase
    {
    public:
        using FileMap = std::unordered_map<GUID, LOADED_TYPE>;
        using ExtensionArray = std::array<std::wstring_view, NUM_EXTENSIONS>;

        Loader(ExtensionArray const& extensions);
        
        LOADED_TYPE const* File(Metadata& metadata) const;
        LOADED_TYPE const* File(GUID guid) const;
        LOADED_TYPE const* File(std::wstring_view const& filename) const;

        FileMap const& Files() const;
        ExtensionArray const& Extensions() const;

        bool SupportsExtension(std::wstring const&) const;

        void LoadFile(std::wstring const& file, std::wstring const& filename) final;

    protected:
        virtual LOADED_TYPE Load(std::wstring const& file) const = 0;

    private:
        FileMap m_files;
        ExtensionArray m_extensions;
    };

    //Function definitions

    template<typename LOADED_TYPE, int NUM_EXTENSIONS>
    inline Loader<LOADED_TYPE, NUM_EXTENSIONS>::Loader(ExtensionArray const& extensions)
        : m_extensions(extensions)
    {
        for (auto& extension : m_extensions)
        {
            AddLoader(*this, extension);
        }
    }

    template<typename LOADED_TYPE, int NUM_EXTENSIONS>
    inline LOADED_TYPE const* Loader<LOADED_TYPE, NUM_EXTENSIONS>::File(Metadata& metadata) const
    {
        if (!metadata.loaded)
        {
            assets::LoadFile(metadata);
        }

        auto it = m_files.find(metadata.guid);
        if (it != m_files.end())
        {
            return &it->second;
        }

        return nullptr;
    }

    template<typename LOADED_TYPE, int NUM_EXTENSIONS>
    inline LOADED_TYPE const* Loader<LOADED_TYPE, NUM_EXTENSIONS>::File(GUID guid) const
    {
        auto* metadata = GetMetadata(guid);
        return metadata != nullptr ? File(*metadata) : nullptr;
    }

    template<typename LOADED_TYPE, int NUM_EXTENSIONS>
    inline LOADED_TYPE const* Loader<LOADED_TYPE, NUM_EXTENSIONS>::File(std::wstring_view const& filename) const
    {
        auto* metadata = GetMetadata(filename);
        return metadata != nullptr ? File(*metadata) : nullptr;
    }

    template<typename LOADED_TYPE, int NUM_EXTENSIONS>
    inline Loader<LOADED_TYPE, NUM_EXTENSIONS>::template FileMap const& Loader<LOADED_TYPE, NUM_EXTENSIONS>::Files() const
    {
        return m_files;
    }

    template<typename LOADED_TYPE, int NUM_EXTENSIONS>
    inline Loader<LOADED_TYPE, NUM_EXTENSIONS>::template ExtensionArray const& Loader<LOADED_TYPE, NUM_EXTENSIONS>::Extensions() const
    {
        return m_extensions;
    }

    template<typename LOADED_TYPE, int NUM_EXTENSIONS>
    inline bool Loader<LOADED_TYPE, NUM_EXTENSIONS>::SupportsExtension(std::wstring const& extension) const
    {
        for (auto& ext : m_extensions)
        {
            if (ext == extension)
            {
                return true;
            }
        }
        return false;
    }

    template<typename LOADED_TYPE, int NUM_EXTENSIONS>
    inline void Loader<LOADED_TYPE, NUM_EXTENSIONS>::LoadFile(std::wstring const& file, std::wstring const& filename)
    {
        m_files.emplace(GUIDFromFilename(filename), Load(file));
    }

}