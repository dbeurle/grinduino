
#pragma once

#include <stdint.h>

namespace lcd1602
{
class display_keypad;
}

namespace og
{
/// timer_preset models an incrementable timer that operates based on poling
/// at 100ms intervals.
class timer_preset
{
public:
    timer_preset(char const* name, uint8_t const eeprom_index, unsigned long const time_ms = 1000);

    auto value() const -> unsigned int { return m_time; }

    /// Increment the timer by 100 ms
    void increment() { m_time += 100; }

    /// Decrement the timer by 100 ms
    void decrement() { m_time = (m_time >= 100) ? m_time - 100 : 0; }

    /// Load a value from the EEPROM of the device \sa save()
    void load();

    /// Load a value from the EEPROM of the device \sa write()
    void save();

    /// Write a value to the display
    /// \param interface Writable interface
    void write(lcd1602::display_keypad& interface) const;

    /// Write a value into the EEPROM of the device
    void write_time(lcd1602::display_keypad& interface) const;

    void start();

    void reset();

    bool is_finished() const;

    void countdown(lcd1602::display_keypad& interface);

private:
    void print_duration(lcd1602::display_keypad& interface, unsigned long const time_ms) const;

private:
    char const* m_name;
    unsigned long m_time = 0;
    unsigned long m_started_at = 0;
    uint8_t m_eeprom_index;
};
}
