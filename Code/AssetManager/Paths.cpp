#include "Paths.h"
#include <filesystem>

std::wstring_view ge2::assets::AssetsPath()
{
    using namespace std::filesystem;

    if (exists(L"../../Assets") && absolute(path(L"../..")).filename() == L"GE2")
    {
        return std::wstring_view(L"../../Assets");
    }
    
    return std::wstring_view(L"./Assets");
}
