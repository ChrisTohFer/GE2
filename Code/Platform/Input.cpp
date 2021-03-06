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

    void Input::Update(WindowMessages const& messages)
    {
        for (ButtonState& bs : m_buttons)
        {
            bs.Down = false;
            bs.Up = false;
        }

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

        Vector2f lastMousePosition = m_mousePosition;

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
                state.Held = true;
            }

            m_mousePosition = { float(me.x), float(me.y) };
        }

        m_mouseMovement = m_mousePosition - lastMousePosition;
    }

}