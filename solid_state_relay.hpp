
#pragma once

namespace og
{
/// solid_state_relay is responsible for interfacing the with SSR that controls
/// the mains energisation state (on / off)
class solid_state_relay
{
public:
    solid_state_relay()
    {
        pinMode(A4, OUTPUT);
        pinMode(A5, OUTPUT);
        off();
    }

    void on()
    {
        m_state = true;
        digitalWrite(A4, HIGH);
        digitalWrite(A5, HIGH);
    }

    void off()
    {
        m_state = false;
        digitalWrite(A4, LOW);
        digitalWrite(A5, LOW);
    }

    bool state() const { return m_state; }

private:
    bool m_state = false;
    int m_pin0, m_pin1;
};
}
