
#pragma once

#include "timer_preset.hpp"
#include "dose_weight.hpp"
#include "solid_state_relay.hpp"

namespace lcd1602
{
class display_keypad;
}

namespace og
{
/// menu_item models a generic single menu item.  Derived classes should
/// override the methods as required.
class menu_item
{
public:
    menu_item(lcd1602::display_keypad* interface) : m_interface(interface) {}

    virtual ~menu_item() = default;

    void attach_up_menu(menu_item* const up) { m_up_menu = up; }

    void attach_down_menu(menu_item* const down) { m_down_menu = down; }

    void attach_upper_menu(menu_item* const upper) { m_upper_menu = upper; }

    void attach_lower_menu(menu_item* const lower) { m_lower_menu = lower; }

    menu_item* on_up() { return m_up_menu; }

    menu_item* on_down() { return m_down_menu; }

    virtual menu_item* on_select() = 0;

    virtual void on_left() = 0;

    virtual void on_right() = 0;

    virtual void show() = 0;

protected:
    lcd1602::display_keypad* const m_interface;

    menu_item *m_up_menu, *m_down_menu;
    menu_item *m_upper_menu = nullptr, *m_lower_menu = nullptr;
};

class timer_preset_item : public menu_item
{
public:
    template <typename... ConstructorArgs>
    timer_preset_item(lcd1602::display_keypad* const interface,
                      solid_state_relay* const motor,
                      ConstructorArgs... args)
        : menu_item(interface), m_motor(motor), m_timer(args...)
    {
        m_timer.load();
    }

    virtual ~timer_preset_item() = default;

    /// Perform the timer loop without blocking the button update.  This enables
    /// the select button presses to be recognised
    virtual menu_item* on_select()
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

    virtual void on_left()
    {
        m_timer.decrement();
        m_timer.write_time(*m_interface);
    }
    virtual void on_right()
    {
        m_timer.increment();
        m_timer.write_time(*m_interface);
    }

    virtual void show() { m_timer.write(*m_interface); }

private:
    solid_state_relay* m_motor;
    timer_preset m_timer;
};

class dose_preset_item : public menu_item
{
public:
    template <typename... ConstructorArgs>
    dose_preset_item(lcd1602::display_keypad* const interface, ConstructorArgs... args)
        : menu_item(interface), m_dose(args...)
    {
        m_dose.load();
    }

    virtual ~dose_preset_item() = default;

    /// Perform the timer loop without blocking the button update.  This enables
    /// the select button presses to be recognised
    virtual menu_item* on_select()
    {
        m_dose.save();
        m_interface->show_saved();
        this->show();

        return this;
    }

    virtual void on_left()
    {
        m_dose.decrement();
        m_dose.write(*m_interface);
    }
    virtual void on_right()
    {
        m_dose.increment();
        m_dose.write(*m_interface);
    }

    virtual void show() { m_dose.write(*m_interface); }

private:
    dose_weight m_dose;
};

class settings : public menu_item
{
public:
    settings(lcd1602::display_keypad* interface) : menu_item(interface) {}

    virtual ~settings() = default;

    virtual menu_item* on_select()
    {
        m_lower_menu->show();
        return m_lower_menu;
    }
    virtual void on_left() { return; }
    virtual void on_right() { return; }

    virtual void show()
    {
        m_interface->display().clear();
        m_interface->display().setCursor(4, 0);
        m_interface->display().print("Settings");
    }
};

class brightness : public menu_item
{
public:
    brightness(lcd1602::display_keypad* interface, int const eeprom_index)
        : menu_item(interface), m_eeprom_index(eeprom_index)
    {
        m_brightness = EEPROM.read(m_eeprom_index);

        pinMode(10, OUTPUT);
        analogWrite(10, m_brightness);
    }

    virtual ~brightness() = default;

    virtual menu_item* on_select()
    {
        // save the output to EEPROM
        EEPROM.write(m_eeprom_index, m_brightness);
        m_interface->show_saved();
        this->show();

        return this;
    }

    virtual void on_left()
    {
        m_brightness = m_brightness < 16 ? 0 : m_brightness - 16;
        analogWrite(10, m_brightness);

        this->show_brightness_bar();
    }

    virtual void on_right()
    {
        m_brightness = m_brightness > 239 ? 255 : m_brightness + 16;
        analogWrite(10, m_brightness);

        this->show_brightness_bar();
    }

    virtual void show() { this->show_brightness_bar(); }

private:
    void show_brightness_bar()
    {
        m_interface->display().clear();
        m_interface->display().setCursor(3, 0);
        m_interface->display().print("Brightness");

        m_interface->display().setCursor(0, 1);
        for (int i = 0; i <= m_brightness / 16; ++i)
        {
            m_interface->display().write(byte(0));
        }
    }

private:
    uint8_t m_brightness = 255;
    int m_eeprom_index;
};

class return_menu : public menu_item
{
public:
    return_menu(lcd1602::display_keypad* interface) : menu_item(interface) {}

    virtual ~return_menu() = default;

    virtual menu_item* on_select()
    {
        m_upper_menu->show();
        delay(100);
        return m_upper_menu;
    }

    virtual void on_left() {}
    virtual void on_right() {}

    virtual void show()
    {
        m_interface->display().clear();
        m_interface->display().setCursor(0, 0);
        m_interface->display().print("To main menu");
    }
};

}
