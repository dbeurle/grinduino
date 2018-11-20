
#include "timer_preset.hpp"

#include "display_keypad.hpp"

#include <Arduino.h>
#include <EEPROM.h>

namespace og
{
timer_preset::timer_preset(char const* name, uint8_t const eeprom_index, unsigned long const time_ms)
    : m_name(name), m_time(time_ms), m_eeprom_index(eeprom_index)
{
}

void timer_preset::load() { m_time = EEPROM.read(m_eeprom_index) * 100; }

void timer_preset::save() { EEPROM.write(m_eeprom_index, m_time / 100); }

void timer_preset::write(lcd1602::display_keypad& interface) const
{
    interface.display().clear();

    // compute the start point
    interface.display().setCursor((16 - strlen(m_name)) / 2, 0);
    interface.display().print(m_name);
    interface.display().setCursor(15, 0);
    interface.display().write(static_cast<byte>(lcd1602::display_keypad::lcd_character::up_down));

    this->print_duration(interface, m_time);

    interface.display().setCursor(2, 1);
    interface.display().print("-");
    interface.display().setCursor(13, 1);
    interface.display().print("+");
}

void timer_preset::write_time(lcd1602::display_keypad& interface) const
{
    this->print_duration(interface, m_time);
}

void timer_preset::start() { m_started_at = millis(); }

void timer_preset::reset() { m_started_at = 0; }

bool timer_preset::is_finished() const { return millis() - m_started_at > m_time; }

void timer_preset::countdown(lcd1602::display_keypad& interface)
{
    this->print_duration(interface, m_time - (millis() - m_started_at));
}

void timer_preset::print_duration(lcd1602::display_keypad& interface, unsigned long const time_ms) const
{
    // maintain the decimal point location for floating point weight
    if (time_ms < 10000)
    {
        interface.display().setCursor(5, 1);
        interface.display().print(" ");
    }
    interface.display().setCursor((time_ms < 10000 ? 6 : 5), 1);
    interface.display().print(time_ms / 1000.0f, 1);
    interface.display().print("s");
}
}
