#include "Window.h"

#include "SFML/Window.hpp"
#include <thread>
#include <mutex>

namespace
{
    ge2::plat::KeyEvent convert(sf::Event::KeyEvent const& keyEvent, ge2::plat::KeyEvent::Type type)
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
    ge2::plat::MouseEvent convert(sf::Event::MouseButtonEvent const& mouseEvent, ge2::plat::MouseEvent::Type type)
    {
        return {
            type,
            static_cast<int>(mouseEvent.button),
            mouseEvent.x,
            mouseEvent.y
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
    public:
        std::atomic<bool>   m_looping = false;
        std::mutex          m_messageMutex;
        std::mutex          m_windowMutex;

        //GE2 window types
        WindowMessages      m_messages;
        WindowConfig        m_config;

        //Window
        sf::Window*         m_window;
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
        if (m_impl && m_impl->m_looping)    //Only attempt to take messages if still looping
        {
            m_impl->m_messageMutex.lock();
            messages = std::move(m_impl->m_messages);
            m_impl->m_messages.closeButtonPressed = false;    //Possibly should be in move assignment/constructor but this is fine
            m_impl->m_messages.gainedFocus        = false;
            m_impl->m_messages.lostFocus          = false;
            m_impl->m_messageMutex.unlock();
        }
        return messages;
    }

    Window::WindowKey Window::CreateKey() const
    {
        return WindowKey(*this);
    }

    //Window::Impl methods

    Window::Impl::Impl(WindowConfig const& config)
        : m_config(config)
    {
        m_messages.width  = config.size.width;
        m_messages.height = config.size.height;
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
        m_window = &window;

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

        //Ensure nobody is holding the mutexes before we exit
        m_messageMutex.lock();
        m_messageMutex.unlock();
        m_windowMutex.lock();
        m_windowMutex.unlock();

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
        auto openGLSettings = sf::ContextSettings(0u, 0u, 0u, m_config.openGLMajorVersion, m_config.openGLMinorVersion);
        return sf::Window(sf::VideoMode(m_config.size.width, m_config.size.height), m_config.title, windowStyle, openGLSettings);
    }

    void Window::Impl::PollEvents(sf::Window& window)
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                //Window updates

            case sf::Event::Closed:
                m_messages.closeButtonPressed = true;
                break;
            case sf::Event::Resized:
                m_messages.width  = m_config.size.width  = event.size.width;
                m_messages.height = m_config.size.height = event.size.height;
                break;
            case sf::Event::LostFocus:
                m_messages.lostFocus = true;
                m_messages.hasFocus  = false;
                break;
            case sf::Event::GainedFocus:
                m_messages.gainedFocus = true;
                m_messages.hasFocus    = true;
                break;

                //Input events (mouse, keyboard)

            case sf::Event::TextEntered:
                m_messages.textInput.push_back(event.text.unicode);
                break;
            case sf::Event::KeyPressed:
                m_messages.keyEvents.push_back(convert(event.key, KeyEvent::Type::PRESSED));
                break;
            case sf::Event::KeyReleased:
                m_messages.keyEvents.push_back(convert(event.key, KeyEvent::Type::RELEASED));
                break;
            case sf::Event::MouseWheelScrolled:
                if (event.mouseWheelScroll.wheel == sf::Mouse::Wheel::VerticalWheel)
                {
                    m_messages.mouseEvents.push_back
                    ({
                        MouseEvent::Type::WHEELSCROLLED,
                        -1,
                        event.mouseWheelScroll.x,
                        event.mouseWheelScroll.y,
                        event.mouseWheelScroll.delta,
                    });
                }
                break;
            case sf::Event::MouseButtonPressed:
                m_messages.mouseEvents.push_back(convert(event.mouseButton, MouseEvent::Type::PRESSED));
                break;
            case sf::Event::MouseButtonReleased:
                m_messages.mouseEvents.push_back(convert(event.mouseButton, MouseEvent::Type::RELEASED));
                break;
            case sf::Event::MouseMoved:
                m_messages.mouseEvents.push_back({ MouseEvent::Type::MOVED, -1, event.mouseMove.x, event.mouseMove.y });
                break;
            case sf::Event::MouseEntered:
                m_messages.mouseEvents.push_back({ MouseEvent::Type::ENTERED });
                break;
            case sf::Event::MouseLeft:
                m_messages.mouseEvents.push_back({ MouseEvent::Type::EXITED });
                break;

                //

            default:
                break;
            }
        }
    }

    Window::WindowKey::WindowKey(ge2::plat::Window const& window)
        : m_window(window.m_impl && window.m_impl->m_looping ? window.m_impl : nullptr)  //Only get access to the window if it is still looping
    {
        if (m_window)
        {
            m_window->m_windowMutex.lock();
            m_window->m_window->setActive(true);
        }
    }
    Window::WindowKey::~WindowKey()
    {
        if (m_window)
        {
            m_window->m_window->setActive(false);
            m_window->m_windowMutex.unlock();
        }
    }

    Window::WindowKey::WindowKey(WindowKey&& rhs)
        : m_window(rhs.m_window)
    {
        rhs.m_window = nullptr;
    }

    Window::WindowKey& Window::WindowKey::operator=(WindowKey&& rhs)
    {
        m_window = rhs.m_window;
        rhs.m_window = nullptr;

        return *this;
    }

    sf::Window* const Window::WindowKey::Window() const
    {
        return m_window ? m_window->m_window : nullptr;
    }

}