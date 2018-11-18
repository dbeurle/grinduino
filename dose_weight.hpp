
#pragma once

#include "display_keypad.hpp"

#include <Arduino.h>
#include <EEPROM.h>

namespace og
{
/// dose_weight is responsible for storing information about the weight in grams
/// of the dose matching the preset grinding time.
class dose_weight
{
public:
    dose_weight(int const eeprom_byte_offset, String&& name)
        : m_eeprom_byte_offset(eeprom_byte_offset), m_name(name)
    {
        this->load();
    }

    /// Increment the timer by 100 ms
    void increment() noexcept { m_value++; }
    /// Decrement the timer by 100 ms
    void decrement() noexcept
    {
        if (m_value == 0)
        {
            return;
        }
        --m_value;
    }
    /// Load a value from the EEPROM of the device \sa save()
    void load() noexcept { m_value = EEPROM.read(m_eeprom_byte_offset); }
    /// Load a value from the EEPROM of the device \sa write()
    void save() noexcept { EEPROM.write(m_eeprom_byte_offset, m_value); }

    /// Write a value to the display
    /// \param interface Writable interface
    void write(lcd1602::display_keypad& interface) const noexcept
    {
        interface.display().clear();
        interface.display().setCursor(0, 0);
        interface.display().print("Dose weight");
        interface.display().setCursor(15, 0);
        interface.display().write(static_cast<byte>(lcd1602::display_keypad::lcd_character::up_down));

        interface.display().setCursor(0, 1);
        interface.display().print(m_name);
        interface.display().print(" = ");

        this->print_weight(interface);
    }

private:
    void print_weight(lcd1602::display_keypad& interface) const noexcept
    {
        interface.display().setCursor(m_name.length() + 3, 1);
        interface.display().print(m_value);
        interface.display().print("g");
    }

private:
    int8_t m_eeprom_byte_offset;
    int8_t m_value;
    String m_name;
};
}
