#include "Assets.h"

#include <filesystem>
#include <iostream>

namespace
{
    std::unordered_map<std::wstring, ge2::ast::Loader*> loaders;

    bool LoadFile(std::wstring const& path)
    {
        auto extensionLocation = path.find_last_of(L".");
        if (extensionLocation != std::string::npos)
        {
            auto extension = path.substr(extensionLocation + 1);
            auto loader = loaders.find(extension);
            if (loader != loaders.end())
            {
                loader->second->LoadFile(path);
                return true;
            }
        }
        return false;
    }

    void IterateDirectory(const wchar_t* directory)
    {
        using namespace std::filesystem;

        for (auto& entry : directory_iterator(directory))
        {
            if (entry.is_directory())
            {
                IterateDirectory(entry.path().c_str());
            }
            else
            {
                auto path = entry.path().wstring();
                if (!LoadFile(path))
                {
                    std::wcout << L"No loader for file:\t" << path << L"\n";
                }
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
