
#pragma once

namespace og
{
/// solid_state_relay is responsible for interfacing the with SSR that controls
/// the mains energisation state (on / off)
class solid_state_relay
{
public:
    solid_state_relay(int const pin = LED_BUILTIN) : m_pin(pin)
    {
        pinMode(m_pin, OUTPUT);
        digitalWrite(m_pin, LOW);
    }

    void on() noexcept
    {
        m_state = true;
        digitalWrite(m_pin, HIGH);
    }

    void off() noexcept
    {
        m_state = false;
        digitalWrite(m_pin, LOW);
    }

    bool state() const noexcept { return m_state; }

private:
    bool m_state = false;
    int m_pin;
};
}
