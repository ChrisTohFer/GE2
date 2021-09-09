#include "AssetManager/Assets.h"
#include "Editor/Editor.h"
#include "Platform/ImguiHelpers.h"
#include "Platform/Window.h"
#include "Graphics/Core.h"

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
        gfx::Initialise();
        ge2::InitialiseImgui(key);

        assets::LoadAllAssets();
    }

    //RuntimeLoop(window);
    RuntimeLoop(window);

    {
        auto key = window.CreateKey();
        ge2::ShutdownImgui();
    }
    window.Close();

    assets::WriteMetadata();
    
    return 0;
}