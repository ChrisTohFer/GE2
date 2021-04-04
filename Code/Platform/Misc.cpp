#include "Misc.h"

#include <Windows.h>

namespace ge2::plat
{
    void SetWorkingDirectoryToExeDirectory()
    {
        char buffer[256];
        GetModuleFileName(nullptr, buffer, 256);
        
        //Remove the file name and keep just the path
        {
            int lastSlash = 0;
            for (int i = 0; i < sizeof(buffer) / sizeof(char); ++i)
            {
                char c = buffer[i];
                if (c == '\\' || c == '/')
                    lastSlash = i;
            }
            buffer[lastSlash] = '\0';
        }
        
        SetCurrentDirectory(buffer);
    }
}