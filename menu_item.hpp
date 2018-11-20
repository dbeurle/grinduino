
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
    menu_item(lcd1602::display_keypad* interface);

    virtual ~menu_item() = default;

    void attach_up_menu(menu_item* const up);

    void attach_down_menu(menu_item* const down);

    void attach_upper_menu(menu_item* const upper);

    void attach_lower_menu(menu_item* const lower);

    menu_item* on_up();

    menu_item* on_down();

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
    virtual menu_item* on_select();

    virtual void on_left();

    virtual void on_right();

    virtual void show();

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
    virtual menu_item* on_select();

    virtual void on_left();

    virtual void on_right();

    virtual void show();

private:
    dose_weight m_dose;
};

class settings : public menu_item
{
public:
    settings(lcd1602::display_keypad* interface);

    virtual ~settings() = default;

    virtual menu_item* on_select();

    virtual void on_left();

    virtual void on_right();

    virtual void show();
};

class brightness : public menu_item
{
public:
    brightness(lcd1602::display_keypad* interface, int const eeprom_index);

    virtual ~brightness() = default;

    virtual menu_item* on_select();

    virtual void on_left();

    virtual void on_right();

    virtual void show();

private:
    void show_brightness_bar();

private:
    uint8_t m_brightness = 255;
    uint8_t m_eeprom_index;
};

class return_menu : public menu_item
{
public:
    return_menu(lcd1602::display_keypad* interface);

    virtual ~return_menu() = default;

    virtual menu_item* on_select();

    virtual void on_left();

    virtual void on_right();

    virtual void show();
};
}
