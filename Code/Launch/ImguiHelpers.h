#pragma once

#include "Platform/Window.h"
#include "Platform/Input.h"

namespace ge2
{
    using namespace plat;

    void InitialiseImgui(Window const&);
    void ShutdownImgui();
    void ImguiBeginFrame(Input const&);
    void ImguiEndFrame();
}