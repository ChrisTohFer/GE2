#include "Assets.h"
#include "Paths.h"

#include <cwctype>
#include <filesystem>
#include <iostream>
#include <unordered_map>

namespace
{
    using LoaderMap = std::unordered_map<std::wstring_view, ge2::assets::Loader*>;
    LoaderMap& Loaders()
    {
        static LoaderMap map;
        return map;
    }

    void LoadFile(std::wstring const& path)
    {
        auto extensionLocation = path.find_last_of(L".");
        if (extensionLocation != std::string::npos)
        {
            auto extension = path.substr(extensionLocation + 1);
            std::transform(extension.begin(), extension.end(), extension.begin(), std::towlower);

            auto loader = Loaders().find(extension);
            if (loader != Loaders().end())
            {
                if (!loader->second->LoadFile(path))
                {
                    std::wcout << L"Failed to load file:\t" << path << L"\n";
                }
                return;
            }
        }
        std::wcout << L"No loader for file:\t" << path << L"\n";
    }

    void IterateDirectory(std::wstring_view directory)
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
    void AddLoader(Loader& loader)
    {
        Loaders().emplace(loader.Extension(), &loader);
    }

    void RemoveLoader(Loader const& loader)
    {
        Loaders().erase(loader.Extension());
    }

    void LoadAllAssets()
    {
        using namespace std::filesystem;

        IterateDirectory(AssetsPath());
    }
}
