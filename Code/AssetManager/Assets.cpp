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
}

namespace ge2::assets
{
    void LoadFile(Metadata& metadata)
    {
        std::filesystem::path path(metadata.path);

        if (metadata.loaded)
        {
            return;
        }

        metadata.loaded = true;

        if (path.has_extension())
        {
            auto extension = path.extension().wstring();
            std::transform(extension.begin(), extension.end(), extension.begin(), std::towlower);

            auto loader = Loaders().find(extension);
            if (loader != Loaders().end())
            {
                loader->second->LoadFile(path, path.filename());
                return;
            }
        }
        std::wcout << L"No loader for file:\t" << path << L"\n";
    }

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
