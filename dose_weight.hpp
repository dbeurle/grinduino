
#pragma once

#include "display_keypad.hpp"

namespace og
{
/// dose_weight is responsible for storing information about the weight in grams
/// of the dose matching the preset grinding time.
class dose_weight
{
public:
    dose_weight(uint8_t const eeprom_byte_offset, char const* name);

    /// Increment the timer by 100 ms
    void increment();

    /// Decrement the timer by 100 ms
    void decrement();

    /// Load a value from the EEPROM of the device \sa save()
    void load();

    /// Load a value from the EEPROM of the device \sa write()
    void save();

    /// Write a value to the display
    /// \param interface Writable interface
    void write(lcd1602::display_keypad& interface) const;

private:
    void print_weight(lcd1602::display_keypad& interface) const;

private:
    uint8_t m_eeprom_byte_offset;
    int8_t m_value;
    char const* m_name;
};
}
