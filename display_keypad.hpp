
#pragma once

#include <stdint.h>

#include <Arduino.h>
#include <LiquidCrystal.h>

namespace lcd1602
{
class display_keypad
{
public:
    enum class button : uint8_t { right, up, down, left, select, none, unknown };

    enum lcd_character { block = 0, blank, up_down };

public:
    explicit display_keypad();

    auto display() -> LiquidCrystal& { return m_lcd; }

    /// Update the button state continuously using a debouncing algorithm
    void update_button_state();

    bool is_select_pressed() const { return m_current == button::select; }

    bool is_left_pressed() const { return m_current == button::left; }

    bool is_right_pressed() const { return m_current == button::right; }

    bool is_up_pressed() const { return m_current == button::up; }

    bool is_down_pressed() const { return m_current == button::down; }

    bool is_none_pressed() const { return m_current == button::none; }

    void show_saved(uint16_t flash_delay = 1000);

private:
    void blank_line(uint8_t const line);

    auto convert_to_code(int const analog_key_input) const -> button;

private:
    LiquidCrystal m_lcd;

    /// Current known state of the switch
    button m_current = button::none;
    /// Last state of the switch
    button m_last = button::none;

    unsigned long m_last_debounce = 0;
};
}
