#pragma once

#include "Platform/Window.h"

namespace ge2::gfx
{
    using namespace plat;

    void Init(Window const&);
    void Shutdown();

    void ClearColour();
    void Display();

}