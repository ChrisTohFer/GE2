#include "Assets.h"

#include <filesystem>
#include <iostream>
#include <cwctype>

namespace
{
    std::unordered_map<std::wstring, ge2::ast::Loader*> loaders;

    void LoadFile(std::wstring const& path)
    {
        auto extensionLocation = path.find_last_of(L".");
        if (extensionLocation != std::string::npos)
        {
            auto extension = path.substr(extensionLocation + 1);
            std::transform(extension.begin(), extension.end(), extension.begin(), std::towlower);

            auto loader = loaders.find(extension);
            if (loader != loaders.end())
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

    void IterateDirectory(const wchar_t* directory)
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

namespace ge2::ast
{
    void AddLoader(Loader& loader)
    {
        loaders.emplace(loader.Extension(), &loader);
    }

    void RemoveLoader(Loader const& loader)
    {
        loaders.erase(loader.Extension());
    }

    void LoadAllAssets()
    {
        using namespace std::filesystem;

        IterateDirectory(L".\\Assets");
    }
}
