#pragma once

#include "Platform/Window.h"
#include "Global/Vector.h"

#include <vector>

namespace ge2::gfx
{
    using namespace plat;

    struct Camera
    {
        Vector3f position = Vector3f::Zero();
        Vector3f rotation = Vector3f::Zero();
        Vector3f positionDelta = Vector3f::Zero();
    };

    void Init();

    void ClearColour();
    void Display(Window::WindowKey const&);
    void Update(int sizeX, int sizeY);

    void LoadTriangle();
    void LoadShaderProgram();
    void DrawTriangle(Vector3f position = { 0,0,0 }, Vector3f rotation = { 0,0,0 });
    void UpdateCamera(Camera&);

}