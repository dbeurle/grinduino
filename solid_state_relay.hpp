
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
        digitalWrite(A4, HIGH);
        digitalWrite(A5, HIGH);
    }

    void off()
    {
        digitalWrite(A4, LOW);
        digitalWrite(A5, LOW);
    }
};
}
