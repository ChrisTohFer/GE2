#include "Temp.h"

#include "glad/glad.h"
#include "SFML/Window.hpp"

namespace
{
    const ge2::plat::Window* window;
}

namespace ge2::gfx
{
    using namespace plat;

    void Init(Window const& windowArg)
    {
        window = &windowArg;

        auto key = window->CreateKey();
        gladLoadGLLoader((GLADloadproc)sf::Context::getFunction);
    }
    void Shutdown()
    {
        window = nullptr;
    }

    void ClearColour()
    {
        auto key = window->CreateKey();
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Display()
    {
        window->CreateKey().Window()->display();
    }

}