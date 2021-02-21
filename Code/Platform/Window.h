#pragma once

#include "Global/Rect.h"

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
    public:
        Window(WindowConfig const& = {});
        ~Window();

        Window(Window const&)            = delete;
        Window(Window &&)                = delete;
        Window& operator=(Window const&) = delete;
        Window& operator=(Window &&)     = delete;

        void Open(WindowConfig const& = {});
        void Close();
        bool IsOpen();

        //Check window events
        bool WasCloseButtonPressed();

    private:
        //Pointer to implementation to avoid unnecessary inclusions in header
        class Impl;
        Impl* m_impl;

        //Data relating to window is all stored in Impl
    };

}