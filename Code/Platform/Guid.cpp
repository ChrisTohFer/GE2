#include "Guid.h"

#include <objbase.h>

ge2::GUID ge2::CreateGuid()
{
    ::GUID winGuid;
    
    ge2::GUID guid = NULL_GUID;
    if (SUCCEEDED(CoCreateGuid(&winGuid)))
    {
        memcpy(&guid, &winGuid, sizeof(guid));
    }

    return guid;
}
