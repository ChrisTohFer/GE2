#pragma once

#include "Camera.h"
#include "Renderer.h"
#include "Platform/Guid.h"
#include "Platform/Window.h"

#include <vector>

namespace ge2::gfx
{
    void Initialise();
    void ClearColour();
    void Display(plat::Window::WindowKey const&);


    void AddRenderer(Transform const&, Renderer const&);
    void Draw(Camera const&, int screenX, int screenY);
}