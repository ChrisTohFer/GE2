#include "Metadata.h"

#include "Loader.h"
#include "Paths.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

namespace
{
    using MetadataMap = std::unordered_map<ge2::GUID, std::wstring>;

    MetadataMap g_metadata;

    std::wstring MetadataFilename()
    {
        return L"metadata.meta";
    }
    std::wstring MetadataFile()
    {
        return std::wstring(ge2::assets::AssetsPath()) + MetadataFilename();
    }
}

namespace ge2::assets
{
    void LoadMetadata()
    {
        g_metadata.clear();

        //Could make this a ifstream and cut out the middleman
        std::wifstream file(MetadataFile());

        std::wstring line;
        while (std::getline(file, line))
        {
            std::wistringstream lineStream(line);

            GUID guid;
            lineStream >> guid;

            std::wstring fileName;
            lineStream >> fileName;

            g_metadata.emplace(guid, fileName);
        }
    }

    void UpdateMetadata()
    {
        //Load current metadata if not already loaded
        if (g_metadata.empty())
        {
            LoadMetadata();
        }

        //Use filesystem to check for new files or missing files
        MetadataMap updatedMap;
        using namespace std::filesystem;
        for (auto& entry : recursive_directory_iterator(AssetsPath()))
        {
            if (entry.is_directory())
            {
                continue;
            }

            auto name = entry.path().filename().wstring();
            if (name == MetadataFilename())
            {
                continue;
            }

            //Check to see if we already know this file, and use the same GUID if we do
            auto guid = GUIDFromFilename(name);
            if (guid == NULL_GUID)
            {
                guid = CreateGuid();
            }
            
            updatedMap.emplace(guid, name);
        }
        g_metadata = updatedMap;

        std::wofstream file(MetadataFile());
        for (auto& entry : updatedMap)
        {
            file << entry.first << L" " << entry.second << L"\n";
        }
    }

    GUID GUIDFromFilename(std::wstring_view const& filename)
    {
        for (auto& entry : g_metadata)
        {
            if (entry.second == filename)
            {
                return entry.first;
            }
        }

        return NULL_GUID;
    }

    std::wstring FilenameFromGUID(GUID guid)
    {
        auto it = g_metadata.find(guid);
        if (it != g_metadata.end())
        {
            return it->second;
        }
        
        return std::wstring(L"");
    }
}
