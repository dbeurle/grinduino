
#include "menu_item.hpp"

#include "display_keypad.hpp"

#include <EEPROM.h>

namespace og
{
menu_item::menu_item(lcd1602::display_keypad* interface) : m_interface(interface) {}

void menu_item::attach_up_menu(menu_item* const up) { m_up_menu = up; }

void menu_item::attach_down_menu(menu_item* const down) { m_down_menu = down; }

void menu_item::attach_upper_menu(menu_item* const upper) { m_upper_menu = upper; }

void menu_item::attach_lower_menu(menu_item* const lower) { m_lower_menu = lower; }

menu_item* menu_item::on_up() { return m_up_menu; }

menu_item* menu_item::on_down() { return m_down_menu; }

menu_item* timer_preset_item::on_select()
{
    delay(250);

    m_timer.save();
    m_timer.start();

    m_motor->on();

    while (!m_timer.is_finished())
    {
        m_interface->update_button_state();

        // Break out of the countdown if user aborted
        if (m_interface->is_select_pressed())
        {
            break;
        }
        m_timer.countdown(*m_interface);
    }
    m_motor->off();
    m_timer.reset();

    this->show();

    return this;
}

void timer_preset_item::on_left()
{
    m_timer.decrement();
    m_timer.write_time(*m_interface);
}

void timer_preset_item::on_right()
{
    m_timer.increment();
    m_timer.write_time(*m_interface);
}

void timer_preset_item::show() { m_timer.write(*m_interface); }

menu_item* dose_preset_item::on_select()
{
    m_dose.save();
    m_interface->show_saved();
    this->show();

    return this;
}

void dose_preset_item::on_left()
{
    m_dose.decrement();
    m_dose.write(*m_interface);
}

void dose_preset_item::on_right()
{
    m_dose.increment();
    m_dose.write(*m_interface);
}

void dose_preset_item::show() { m_dose.write(*m_interface); }

menu_item* settings::on_select()
{
    m_lower_menu->show();
    return m_lower_menu;
}

settings::settings(lcd1602::display_keypad* interface) : menu_item(interface) {}

void settings::on_left() { return; }

void settings::on_right() { return; }

void settings::show()
{
    m_interface->display().clear();
    m_interface->display().setCursor(4, 0);
    m_interface->display().print(F("Settings"));
    m_interface->display().setCursor(15, 0);
    m_interface->display().write(static_cast<byte>(lcd1602::display_keypad::lcd_character::up_down));
}

brightness::brightness(lcd1602::display_keypad* interface, int const eeprom_index)
    : menu_item(interface), m_eeprom_index(eeprom_index)
{
    m_brightness = EEPROM.read(m_eeprom_index);

    pinMode(10, OUTPUT);
    analogWrite(10, m_brightness);
}

menu_item* brightness::on_select()
{
    // save the output to EEPROM
    EEPROM.write(m_eeprom_index, m_brightness);
    m_interface->show_saved();
    this->show();

    return this;
}

void brightness::on_left()
{
    m_brightness = m_brightness < 16 ? 0 : m_brightness - 16;
    analogWrite(10, m_brightness);

    this->show_brightness_bar();
}

void brightness::on_right()
{
    m_brightness = m_brightness > 239 ? 255 : m_brightness + 16;
    analogWrite(10, m_brightness);

    this->show_brightness_bar();
}

void brightness::show() { this->show_brightness_bar(); }

void brightness::show_brightness_bar()
{
    m_interface->display().clear();
    m_interface->display().setCursor(3, 0);
    m_interface->display().print(F("Brightness"));
    m_interface->display().setCursor(15, 0);
    m_interface->display().write(static_cast<byte>(lcd1602::display_keypad::lcd_character::up_down));

    m_interface->display().setCursor(0, 1);
    for (int i = 0; i <= m_brightness / 16; ++i)
    {
        m_interface->display().write(static_cast<byte>(lcd1602::display_keypad::lcd_character::block));
    }
}

return_menu::return_menu(lcd1602::display_keypad* interface) : menu_item(interface) {}

menu_item* return_menu::on_select()
{
    m_upper_menu->show();
    delay(100);
    return m_upper_menu;
}

void return_menu::on_left() { return; }

void return_menu::on_right() { return; }

void return_menu::show()
{
    m_interface->display().clear();
    m_interface->display().setCursor(0, 0);
    m_interface->display().print(F("To main menu"));

    m_interface->display().setCursor(15, 0);
    m_interface->display().write(static_cast<byte>(lcd1602::display_keypad::lcd_character::up_down));
}
}
