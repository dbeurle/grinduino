
#pragma once

#include <stdint.h>

#include <Arduino.h>
#include <LiquidCrystal.h>

namespace lcd1602
{
class display_keypad
{
public:
    enum class button : int8_t { right, up, down, left, select, none, unknown };

    enum lcd_character { block = 0, blank, up_down };

public:
    display_keypad() : m_lcd(8, 9, 4, 5, 6, 7)
    {
        m_lcd.begin(16, 2);

        byte block[8] = {B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111};
        byte blank[8] = {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00000};
        byte up_down[8] = {B00100, B01010, B10001, B00000, B00000, B10001, B01010, B00100};

        m_lcd.createChar(lcd_character::block, block);
        m_lcd.createChar(lcd_character::blank, blank);
        m_lcd.createChar(lcd_character::up_down, up_down);
    }

    auto display() -> LiquidCrystal& { return m_lcd; }

    /// Update the button state continuously using a debouncing algorithm
    void update_button_state()
    {
        m_last = convert_to_code(analogRead(A0));

        // No change in the switch state
        if (m_current != m_last)
        {
            // Check if the state has changed in the last 50 ms
            if (millis() - m_last_debounce > 50)
            {
                m_current = m_last;
                m_last_debounce = millis();
                m_has_state_changed = true;
            }
        }
        else
        {
            m_has_state_changed = false;
        }
    }

    bool is_select_pressed() const { return m_current == button::select; }

    bool is_left_pressed() const { return m_current == button::left; }

    bool is_right_pressed() const { return m_current == button::right; }

    bool is_up_pressed() const { return m_current == button::up; }

    bool is_down_pressed() const { return m_current == button::down; }

    bool is_none_pressed() const { return m_current == button::none; }

    bool has_state_changed() const { return m_has_state_changed; }

    void show_saved(uint32_t flash_delay = 1000)
    {
        m_lcd.clear();
        m_lcd.setCursor(5, 0);
        m_lcd.print(F("Saved!"));
        delay(flash_delay);
    }

private:
    auto blank_line(uint8_t const line) -> void
    {
        m_lcd.setCursor(0, line == 1);

        for (int i = 0; i < 16; ++i)
        {
            m_lcd.write(static_cast<byte>(lcd_character::blank));
        }
    }

    auto convert_to_code(int const analog_key_input) const -> button
    {
        // Most likely value is nothing pressed
        if (analog_key_input > 1000)
        {
            return button::none;
        }
        else if (analog_key_input <= 50)
        {
            return button::right;
        }
        else if (analog_key_input <= 150)
        {
            return button::up;
        }
        else if (analog_key_input <= 350)
        {
            return button::down;
        }
        else if (analog_key_input <= 500)
        {
            return button::left;
        }
        else if (analog_key_input >= 550 && analog_key_input < 700)
        {
            return button::select;
        }
        return button::none;
    }

private:
    LiquidCrystal m_lcd;

    /// Current known state of the switch
    button m_current = button::none;
    /// Last state of the switch
    button m_last = button::none;

    bool m_has_state_changed = false;

    unsigned long m_last_debounce = 0;
};
}
