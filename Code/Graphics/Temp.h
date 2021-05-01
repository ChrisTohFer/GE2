#pragma once

#include "Platform/Window.h"
#include "Global/Vector.h"

#include <vector>

namespace ge2::gfx
{
    using namespace plat;

    void Init();

    void ClearColour();
    void Display(Window::WindowKey const&);
    void Update(int sizeX, int sizeY);

    void LoadTriangle();
    void LoadShaderProgram();
    void LoadTexture();
    void DrawTriangle(Vector3f position = { 0,0,0 }, Vector3f rotation = { 0,0,0 });

}