
#include "display_keypad.hpp"

namespace lcd1602
{
display_keypad::display_keypad() : m_lcd(8, 9, 4, 5, 6, 7)
{
    m_lcd.begin(16, 2);

    byte block[8] = {B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111};
    byte blank[8] = {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00000};
    byte up_down[8] = {B00100, B01010, B10001, B00000, B00000, B10001, B01010, B00100};

    m_lcd.createChar(lcd_character::block, block);
    m_lcd.createChar(lcd_character::blank, blank);
    m_lcd.createChar(lcd_character::up_down, up_down);
}

void display_keypad::update_button_state()
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
        }
    }
}

void display_keypad::show_saved(uint16_t flash_delay)
{
    m_lcd.clear();
    m_lcd.setCursor(5, 0);
    m_lcd.print(F("Saved!"));
    delay(flash_delay);
}

void display_keypad::blank_line(uint8_t const line)
{
    m_lcd.setCursor(0, line == 1);

    for (int i = 0; i < 16; ++i)
    {
        m_lcd.write(static_cast<byte>(lcd_character::blank));
    }
}

auto display_keypad::convert_to_code(int const analog_key_input) const -> button
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

}
