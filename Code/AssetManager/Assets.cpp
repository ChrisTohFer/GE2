#include "Assets.h"
#include "Paths.h"

#include <cwctype>
#include <filesystem>
#include <iostream>
#include <unordered_map>

namespace
{
    using LoaderMap = std::unordered_map<std::wstring_view, ge2::assets::LoaderBase*>;
    LoaderMap& Loaders()
    {
        static LoaderMap map;
        return map;
    }

    void LoadFile(std::filesystem::path const& path)
    {
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

    void IterateDirectory(std::wstring_view const& directory)
    {
        using namespace std::filesystem;

        for (auto& entry : recursive_directory_iterator(directory))
        {
            if (!entry.is_directory())
            {
                LoadFile(entry.path().wstring());
            }
        }
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

        IterateDirectory(AssetsPath());
    }
}
