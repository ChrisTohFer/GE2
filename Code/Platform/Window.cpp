#include "Window.h"

#include "SFML/Window.hpp"
#include <thread>

namespace ge2::plat {

    class Window::Impl {
    public:
        Impl(WindowConfig const&);
        ~Impl();

    private:
        void Loop();
        void PollEvents(sf::Window&);

        sf::Window CreateWindow();

        //Thread stuff
        std::thread         m_thread;
        bool                m_looping = false;

    public:
        //Window messaging
        bool                m_closeButtonPressed = false;

        //Other
        WindowConfig        m_config;
    };

    //Window methods

    Window::Window(WindowConfig const& config)
    {
        Open(config);
    }

    Window::~Window()
    {
        Close();
    }

    void Window::Open(WindowConfig const& config)
    {
        m_impl = new Impl(config);
    }

    void Window::Close()
    {
        delete m_impl;  //Will wait for Impl thread to end
        m_impl = nullptr;
    }

    bool Window::IsOpen()
    {
        return m_impl;
    }

    bool Window::WasCloseButtonPressed()
    {
        return m_impl ? m_impl->m_closeButtonPressed : false;
    }

    //Window::Impl methods

    Window::Impl::Impl(WindowConfig const& config)
        : m_config(config)
    {
        m_thread = std::move(std::thread(&Window::Impl::Loop, this));
        while (!m_looping) {};  //Wait until Loop has started before continuing
    }

    Window::Impl::~Impl()
    {
        m_looping = false;
        m_thread.join();
    }

    void Window::Impl::Loop()
    {
        sf::Window window = CreateWindow();

        window.setActive(false);
        m_looping = true;
        while (m_looping)
        {
            PollEvents(window);

            using namespace std::chrono_literals;
            std::this_thread::sleep_for(5ms);
        }

        window.setActive(true);
        window.close();
    }

    sf::Window Window::Impl::CreateWindow()
    {
        auto windowStyle = sf::Style::None;
        switch (m_config.style)
        {
        case WindowConfig::Style::NORMAL:
        case WindowConfig::Style::FULLSCREEN:   //Fullscreen not yet supported, just treat same as default
            windowStyle = sf::Style::Default;
            break;
        case WindowConfig::Style::BORDERLESS:
            break;
        case WindowConfig::Style::BORDERLESS_FULLSCREEN:
            //config.size = screenDimensions    -   TODO
            break;
        }
        return sf::Window(sf::VideoMode(m_config.size.width, m_config.size.height), m_config.title, windowStyle);
    }

    void Window::Impl::PollEvents(sf::Window& window)
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                m_closeButtonPressed = true;
        }
    }

}