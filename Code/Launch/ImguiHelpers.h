#pragma once

#include "Platform/Window.h"
#include "Platform/Input.h"

namespace ge2
{
    using namespace plat;

    void InitialiseImgui(Window::WindowKey const&);
    void ShutdownImgui(Window::WindowKey const&);
    void ImguiBeginFrame(Window::WindowKey const&, Input const&);
    void ImguiEndFrame(Window::WindowKey const&);
}