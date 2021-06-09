#include "Metadata.h"

#include "Loader.h"
#include "Paths.h"

#include "Platform/Guid.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace ge2::assets
{
    namespace
    {
        std::wstring MetadataFilename()
        {
            return L"metadata.meta";
        }
        std::wstring MetadataFile()
        {
            return std::wstring(AssetsPath()) + MetadataFilename();
        }

        bool LoadMetadata();

        MetadataMap g_metadata;
        bool g_loadAtStaticInitCheese = LoadMetadata();

        void WriteMetadata(std::wostream& output, MetadataMap& map)
        {
            for (auto& entry : map)
            {
                auto& guid = entry.first;
                auto& metadata = entry.second;
                output << guid << L" " << metadata.name;

                for (auto const& dependency : metadata.dependencies)
                {
                    output << L" " << dependency;
                }

                output << L"\n";
            }
        }

        void UpdateMetadata()
        {
            //Use filesystem to check existing metadata against files
            MetadataMap updatedMap;
            using namespace std::filesystem;
            for (auto& entry : recursive_directory_iterator(AssetsPath()))
            {
                if (entry.is_directory())
                {
                    continue;
                }

                auto& path = entry.path();
                auto name = path.filename().wstring();
                if (name == MetadataFilename())
                {
                    continue;
                }

                //Check to see if we already know this file
                auto guid = GUIDFromFilename(name);
                if (guid != NULL_GUID)
                {
                    //Update existing data in case path/extension aren't already known
                    Metadata metadata = g_metadata[guid];
                    metadata.path = path.wstring();
                    metadata.extension = path.extension().wstring();

                    updatedMap.emplace(guid, metadata);
                }
                else
                {
                    //Create new data
                    guid = CreateGuid();
                    updatedMap.emplace(guid, Metadata{ guid, name, {}, path.wstring(), path.extension().wstring() });
                }
            }

            //Mark old metadata as missing if we didn't find the file
            for (auto& entry : g_metadata)
            {
                auto it = updatedMap.find(entry.first);
                if (it == updatedMap.end())
                {
                    it->second.missing = true;
                    updatedMap.emplace(it->first, it->second);
                }
            }

            g_metadata = updatedMap;
        }

        //Load metadata from the metadata file
        bool LoadMetadata()
        {
            std::wifstream file(MetadataFile());

            std::wstring line;
            while (std::getline(file, line))
            {
                std::wistringstream lineStream(line);

                Metadata entry;
                lineStream >> entry.guid;
                lineStream >> entry.name;

                GUID dependency;
                while (lineStream >> dependency)
                {
                    entry.dependencies.push_back(dependency);
                }

                g_metadata.emplace(entry.guid, entry);
            }

            //Use update to update path and extension fields, and remove metadata for non-existing files
            UpdateMetadata();

            //Returning a value just so we can run this at static init
            return true;
        }
    }

    void WriteMetadata()
    {
        UpdateMetadata();

        std::wofstream file(MetadataFile());
        WriteMetadata(file, g_metadata);
    }

    GUID GUIDFromFilename(std::wstring_view const& filename)
    {
        for (auto& entry : g_metadata)
        {
            if (entry.second.name == filename)
            {
                return entry.first;
            }
        }

        _ASSERT(false); //Attempting to load non existing metadata

        return NULL_GUID;
    }

    MetadataMap& GetAllMetadata()
    {
        return g_metadata;
    }

    Metadata* GetMetadata(GUID guid)
    {
        auto it = g_metadata.find(guid);
        if (it != g_metadata.end())
        {
            return &it->second;
        }

        _ASSERT(false); //Attempting to load non existing metadata
        
        return nullptr;
    }

    Metadata* AddMetadata()
    {
        auto guid = ge2::CreateGuid();
        g_metadata.emplace(guid, Metadata{ guid });
        return &g_metadata[guid];
    }

    void RemoveMetadata(GUID guid)
    {
        g_metadata.erase(guid);
    }
}
