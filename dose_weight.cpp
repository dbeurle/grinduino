
#include "dose_weight.hpp"

#include <EEPROM.h>

namespace og
{
dose_weight::dose_weight(uint8_t const eeprom_byte_offset, String&& name)
    : m_eeprom_byte_offset(eeprom_byte_offset), m_name(name + " weight")
{
    this->load();
}

void dose_weight::increment() { m_value++; }

void dose_weight::decrement()
{
    if (m_value == 0)
    {
        return;
    }
    --m_value;
}

void dose_weight::load() { m_value = EEPROM.read(m_eeprom_byte_offset); }

void dose_weight::save() { EEPROM.write(m_eeprom_byte_offset, m_value); }

void dose_weight::write(lcd1602::display_keypad& interface) const
{
    interface.display().clear();
    interface.display().setCursor((16 - m_name.length()) / 2, 0);
    interface.display().print(m_name);

    interface.display().setCursor(15, 0);
    interface.display().write(static_cast<byte>(lcd1602::display_keypad::lcd_character::up_down));

    interface.display().setCursor(2, 1);
    interface.display().print("-");
    interface.display().setCursor(13, 1);
    interface.display().print("+");

    this->print_weight(interface);
}

void dose_weight::print_weight(lcd1602::display_keypad& interface) const
{
    interface.display().setCursor(m_value >= 10 ? 6 : 7, 1);
    interface.display().print(m_value);
    interface.display().print(F("g"));
}

}
