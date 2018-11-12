
#pragma once

namespace og
{
/// timer_preset models an incrementable timer that operates based on poling
/// at 100ms intervals.
class timer_preset
{
public:
    timer_preset(char const* name, int const eeprom_index, unsigned long const time_ms = 1000)
        : m_name(name), m_time(time_ms), m_eeprom_index(eeprom_index)
    {
    }

    auto value() const -> unsigned int { return m_time; }

    /// Increment the timer by 100 ms
    void increment() { m_time += 100; }
    /// Decrement the timer by 100 ms
    void decrement() { m_time = (m_time >= 100) ? m_time - 100 : 0; }

    /// Load a value from the EEPROM of the device \sa save()
    void load() noexcept { m_time = EEPROM.read(m_eeprom_index) * 100; }
    /// Load a value from the EEPROM of the device \sa write()
    void save() noexcept { EEPROM.write(m_eeprom_index, m_time / 100); }

    /// Write a value to the display
    /// \param interface Writable interface
    void write(lcd1602::display_keypad& interface) const noexcept
    {
        interface.display().clear();
        interface.display().setCursor(0, 0);
        interface.display().print(m_name);

        interface.display().setCursor(0, 1);
        interface.display().print("Duration = ");

        this->print_duration(interface, m_time);
    }

    /// Write a value into the EEPROM of the device
    void write_time(lcd1602::display_keypad& interface) const noexcept
    {
        this->print_duration(interface, m_time);
    }

    void start() noexcept { m_started_at = millis(); }

    void reset() noexcept { m_started_at = 0; }

    bool is_finished() const { return millis() - m_started_at > m_time; }

    void countdown(lcd1602::display_keypad& interface)
    {
        this->print_duration(interface, m_time - (millis() - m_started_at));
    }

private:
    void print_duration(lcd1602::display_keypad& interface, unsigned long const time_ms) const
    {
        if (time_ms < 10000)
        {
            interface.display().setCursor(15, 1);
            interface.display().print(" ");
        }

        interface.display().setCursor(11, 1);
        interface.display().print(time_ms / 1000.0f, 1);
        interface.display().print("s");
    }

private:
    char const* m_name;
    unsigned long m_time = 0;
    unsigned long m_started_at = 0;
    int m_eeprom_index;
};
}
