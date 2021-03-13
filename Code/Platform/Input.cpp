#include "Input.h"

namespace ge2::plat
{

    bool Input::ButtonDown(Button button) const
    {
        return m_buttons[button].Down;
    }

    bool Input::ButtonUp(Button button) const
    {
        return m_buttons[button].Up;
    }

    bool Input::ButtonHeld(Button button) const
    {
        return m_buttons[button].Held;
    }

    Vector2f Input::MousePosition() const
    {
        return m_mousePosition;
    }

    Vector2f Input::MouseDelta() const
    {
        return m_mouseMovement;
    }

    float Input::MouseWheelDelta() const
    {
        return m_mouseWheelDelta;
    }

    std::vector<unsigned int> Input::UnicodeInput() const
    {
        return m_unicodeInput;
    }

    bool Input::WindowHasFocus() const
    {
        return m_windowHasFocus;
    }

    void Input::Update(WindowMessages& messages)
    {
        for (ButtonState& bs : m_buttons)
        {
            bs.Down = false;
            bs.Up = false;
        }

        //Text

        m_unicodeInput = std::move(messages.textInput);

        //Keyboard

        for (KeyEvent const& ke : messages.keyEvents)
        {
            if (ke.key == KeyEvent::Key::Unknown)
                continue;

            auto& state = m_buttons[int(ke.key)];
            if (ke.type == KeyEvent::Type::PRESSED)
            {
                state.Down = true;
                state.Held = true;
            }
            else
            {
                state.Up = true;
                state.Held = false;
            }
        }

        //Mouse

        Vector2f lastMousePosition = m_mousePosition;
        m_mouseWheelDelta = 0;
        for (MouseEvent const& me : messages.mouseEvents)
        {
            auto& state = m_buttons[int(Button::MouseLeft) + me.button];

            if (me.type == MouseEvent::Type::PRESSED)
            {
                state.Down = true;
                state.Held = true;
            }
            else if (me.type == MouseEvent::Type::RELEASED)
            {
                state.Up = true;
                state.Held = false;
            }
            else if (me.type == MouseEvent::Type::WHEELSCROLLED)
            {
                m_mouseWheelDelta += me.wheelScroll;
            }

            m_mousePosition = { float(me.x), float(me.y) };
        }
        m_mouseMovement = m_mousePosition - lastMousePosition;

        //Other
        m_windowHasFocus = messages.hasFocus;
    }

}