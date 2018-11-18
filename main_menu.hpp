
#pragma once

#include "menu_item.hpp"
#include "persistent_storage.hpp"

namespace og
{
class main_menu
{
public:
    main_menu(lcd1602::display_keypad* const interface, solid_state_relay* const motor);

    ~main_menu();

    void on_select() { m_current_item = m_current_item->on_select(); }

    void on_left() { m_current_item->on_left(); }

    void on_right() { m_current_item->on_right(); }

    void on_up()
    {
        m_current_item = m_current_item->on_up();
        m_current_item->show();
    }

    void on_down()
    {
        m_current_item = m_current_item->on_down();
        m_current_item->show();
    }

    void show() { m_current_item->show(); }

private:
    lcd1602::display_keypad* m_interface;

    menu_item* m_items[9];
    menu_item* m_current_item = nullptr;
};
}
