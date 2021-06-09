#include "Assets.h"
#include "Metadata.h"
#include "Paths.h"

#include <cwctype>
#include <filesystem>
#include <iostream>
#include <unordered_map>

namespace
{
    using namespace ge2::assets;

    //Map extensions to appropriate loader
    using LoaderMap = std::unordered_map<std::wstring_view, LoaderBase*>;
    LoaderMap& Loaders()
    {
        static LoaderMap map;
        return map;
    }

    bool LoadFile(Metadata& metadata)
    {
        if (metadata.loadStarted)
        {
            _ASSERT(metadata.loaded); //Circular dependency
            return true;
        }

        for (auto& dependency : metadata.dependencies)
        {
            auto metadata = GetMetadata(dependency);
            _ASSERT(metadata);      //Failed to load metadata of dependency
            if (metadata)
            {
                auto success = LoadFile(*metadata);
                _ASSERT(success);   //Failed to load dependency
            }
        }

        std::filesystem::path path(metadata.path);
        if (path.has_extension())
        {
            auto extension = path.extension().wstring();
            std::transform(extension.begin(), extension.end(), extension.begin(), std::towlower);

            auto loader = Loaders().find(extension);
            if (loader != Loaders().end())
            {
                loader->second->LoadFile(path, path.filename());
                return true;
            }
        }
        std::wcout << L"No loader for file:\t" << path << L"\n";

        return false;
    }
}

namespace ge2::assets
{
    void AddLoader(LoaderBase& loader, std::wstring_view const& extension)
    {
        Loaders().emplace(extension, &loader);
    }

    void LoadAllAssets()
    {
        using namespace std::filesystem;

        for (auto& entry : GetAllMetadata())
        {
            LoadFile(entry.second);
        }
    }
}
