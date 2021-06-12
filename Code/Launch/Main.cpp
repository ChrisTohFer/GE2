#include "AssetManager/Assets.h"
#include "Editor/Editor.h"
#include "Platform/ImguiHelpers.h"
#include "Platform/Window.h"
#include "Graphics/Temp.h"

#include <thread>

namespace ge2
{
    void RuntimeLoop(plat::Window& window);
}

int main()
{
    using namespace ge2;
    using namespace ge2::plat;

    Window window;

    {
        auto key = window.CreateKey();
        gfx::Init();
        ge2::InitialiseImgui(key);
        gfx::LoadTriangle();
        gfx::LoadShaderProgram();

        assets::LoadAllAssets();
    }

    //RuntimeLoop(window);
    EditorLoop(window);

    {
        auto key = window.CreateKey();
        ge2::ShutdownImgui();
    }
    window.Close();
    
    return 0;
}