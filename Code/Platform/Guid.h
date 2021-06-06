#pragma once

namespace ge2
{
    using GUID = unsigned long long;

    constexpr GUID NULL_GUID = 0ull;

    GUID CreateGuid();
}