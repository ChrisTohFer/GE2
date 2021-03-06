#pragma once

#include "WindowMessages.h"
#include "Global/Rect.h"

namespace sf
{
    class Window;
}

namespace ge2::plat {

    struct WindowConfig
    {
        enum class Style
        {
            NORMAL,
            FULLSCREEN,
            BORDERLESS,
            BORDERLESS_FULLSCREEN
        };

        const char* title  = "Window";
        Style       style = Style::NORMAL;
        IntRect       size  = { 0,0,500,500 };
        //Possibly something for opengl parameters here
    };

    class Window
    {
        class Impl;
    public:
        class WindowKey     //The thread holding this key can use the window for any purpose except message handling until the key is destroyed
        {
        public:
            WindowKey(Window&);
            ~WindowKey();

            sf::Window* Window();
        private:
            Impl* m_window = nullptr;
        };

        Window(WindowConfig const& = {});
        ~Window();

        Window(Window const&)            = delete;
        Window(Window &&)                = delete;
        Window& operator=(Window const&) = delete;
        Window& operator=(Window &&)     = delete;

        void Open(WindowConfig const& = {});
        void Close();
        bool IsOpen() const;

        //Get window events since last call of TakeMessages()
        WindowMessages TakeMessages();

    private:
        //Pointer to implementation to avoid unnecessary inclusions in header
        Impl* m_impl;

        //Data relating to window is all stored in Impl
    };

}