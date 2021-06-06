#include "Loader.h"

#include <fstream>

std::string ge2::assets::LoadTextFile(std::wstring const& fileName)
{
    //Get file size
    std::ifstream file(fileName);
    const auto begin = file.tellg();
    file.seekg(0, std::ios::end);
    const auto end = file.tellg();
    const auto fsize = (end - begin);

    //Read from beginning
    file.seekg(begin);

    std::string content;
    content.resize(fsize, '\0');
    file.read(content.data(), fsize);

    return content;
}
