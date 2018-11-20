
#include "main_menu.hpp"

namespace og
{
main_menu::main_menu(lcd1602::display_keypad* const interface, solid_state_relay* const motor)
    : m_interface(interface),
      m_items{new timer_preset_item(interface, motor, "Purge", persistent_storage::allocate()),
              new timer_preset_item(interface, motor, "Single dose", persistent_storage::allocate()),
              new timer_preset_item(interface, motor, "Double dose", persistent_storage::allocate()),
              new settings(interface),
              new dose_preset_item(interface, persistent_storage::allocate(), "Purge"),
              new dose_preset_item(interface, persistent_storage::allocate(), "Single"),
              new dose_preset_item(interface, persistent_storage::allocate(), "Double"),
              new brightness(interface, persistent_storage::allocate()),
              new return_menu(interface)}
{
    // timer presets
    m_items[0]->attach_up_menu(m_items[3]);
    m_items[0]->attach_down_menu(m_items[1]);
    m_items[1]->attach_up_menu(m_items[0]);
    m_items[1]->attach_down_menu(m_items[2]);
    m_items[2]->attach_up_menu(m_items[1]);
    m_items[2]->attach_down_menu(m_items[3]);

    // settings menu
    m_items[3]->attach_up_menu(m_items[2]);
    m_items[3]->attach_down_menu(m_items[0]);
    m_items[3]->attach_lower_menu(m_items[4]);

    // dose weight
    m_items[4]->attach_up_menu(m_items[8]);
    m_items[4]->attach_down_menu(m_items[5]);
    m_items[5]->attach_up_menu(m_items[4]);
    m_items[5]->attach_down_menu(m_items[6]);
    m_items[6]->attach_up_menu(m_items[5]);
    m_items[6]->attach_down_menu(m_items[7]);

    // brightness
    m_items[7]->attach_up_menu(m_items[6]);
    m_items[7]->attach_down_menu(m_items[8]);

    // return
    m_items[8]->attach_up_menu(m_items[7]);
    m_items[8]->attach_down_menu(m_items[4]);
    m_items[8]->attach_upper_menu(m_items[0]);

    m_current_item = m_items[0];
}

main_menu::~main_menu()
{
    for (auto* item : m_items)
    {
        delete item;
        item = nullptr;
    }
}
}
