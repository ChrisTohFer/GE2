#include "Window.h"

#include "SFML/Window.hpp"
#include <thread>
#include <mutex>

namespace
{
    ge2::plat::KeyEvent convert(sf::Event::KeyEvent keyEvent, ge2::plat::KeyEvent::Type type)
    {
        return {
            type,
            static_cast<ge2::plat::KeyEvent::Key>(static_cast<int>(keyEvent.code)),
            keyEvent.alt,
            keyEvent.control,
            keyEvent.shift,
            keyEvent.system
        };
    }
}

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
        std::mutex          m_messageMutex;

        //Window messaging
        WindowMessages      m_messages;

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

    bool Window::IsOpen() const
    {
        return m_impl;
    }

    WindowMessages Window::TakeMessages()
    {
        WindowMessages messages;
        if (m_impl)
        {
            m_impl->m_messageMutex.lock();
            messages = std::move(m_impl->m_messages);
            m_impl->m_messageMutex.unlock();
        }
        return messages;
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
            m_messageMutex.lock();
            PollEvents(window);
            m_messageMutex.unlock();

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
            switch (event.type)
            {
            case sf::Event::Closed:
                m_messages.closeButtonPressed = true;
                break;
            case sf::Event::KeyPressed:
                m_messages.keyEvents.push_back(convert(event.key, KeyEvent::Type::PRESSED));
                break;
            case sf::Event::KeyReleased:
                m_messages.keyEvents.push_back(convert(event.key, KeyEvent::Type::RELEASED));
                break;

            default:
                break;
            }
        }
    }

}