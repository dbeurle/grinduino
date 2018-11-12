
#include <EEPROM.h>
#include <LiquidCrystal.h>

#include "persistent_storage.hpp"
#include "display_keypad.hpp"
#include "main_menu.hpp"
#include "solid_state_relay.hpp"
#include "dose_weight.hpp"

lcd1602::display_keypad interface;

og::dose_weight purge_weight(og::persistent_storage{}.allocate(), "Purge");
og::dose_weight single_weight(og::persistent_storage{}.allocate(), "Single Dose");
og::dose_weight double_weight(og::persistent_storage{}.allocate(), "Double Dose");

og::solid_state_relay motor(13);

/// Print a cute welcome message
static void welcome()
{
    interface.display().clear();
    interface.display().setCursor(2, 0);
    interface.display().print("Open Grinder");
    interface.display().setCursor(2, 1);
    interface.display().print("Version 0.0.1");
    delay(1500);
}

og::main_menu menu(&interface, &motor);

void setup()
{
    interface.display().begin(16, 2);
    welcome();
    menu.show();
}

void loop()
{
    interface.update_button_state();

    if (interface.is_none_pressed())
    {
        return;
    }

    if (interface.is_down_pressed())
    {
        menu.on_down();
    }
    else if (interface.is_up_pressed())
    {
        menu.on_up();
    }
    else if (interface.is_left_pressed())
    {
        menu.on_left();
    }
    else if (interface.is_right_pressed())
    {
        menu.on_right();
    }
    else if (interface.is_select_pressed())
    {
        menu.on_select();
    }
    delay(150);
}

// if (interface.is_down_pressed())
// {
//     if (item == og::menu::purge)
//     {
//         item = og::menu::single_dose;
//         single_preset.write(interface);
//     }
//     else if (item == og::menu::single_dose)
//     {
//         item = og::menu::double_dose;
//         double_preset.write(interface);
//     }
//     else if (item == og::menu::double_dose)
//     {
//         item = og::menu::settings;
//         interface.display().clear();
//         interface.display().setCursor(0, 0);
//         interface.display().print("settings");
//     }
//     else if (item == og::menu::settings)
//     {
//         item = og::menu::purge;
//         purge_preset.write(interface);
//     }
// }
// else if (interface.is_up_pressed())
// {
//     if (item == og::menu::purge)
//     {
//         item = og::menu::settings;
//         interface.display().clear();
//         interface.display().setCursor(0, 0);
//         interface.display().print("settings");
//     }
//     else if (item == og::menu::single_dose)
//     {
//         item = og::menu::purge;
//         purge_preset.write(interface);
//     }
//     else if (item == og::menu::double_dose)
//     {
//         item = og::menu::single_dose;
//         single_preset.write(interface);
//     }
//     else if (item == og::menu::settings)
//     {
//         item = og::menu::double_dose;
//         double_preset.write(interface);
//     }
// }
// else if (interface.is_left_pressed())
// {
//     if (item == og::menu::purge)
//     {
//         purge_preset.decrement();
//         purge_preset.write_time(interface);
//     }
//     else if (item == og::menu::single_dose)
//     {
//         single_preset.decrement();
//         single_preset.write_time(interface);
//     }
//     else if (item == og::menu::double_dose)
//     {
//         double_preset.decrement();
//         double_preset.write_time(interface);
//     }
// }
// else if (interface.is_right_pressed())
// {
//     if (item == og::menu::purge)
//     {
//         purge_preset.increment();
//         purge_preset.write_time(interface);
//     }
//     else if (item == og::menu::single_dose)
//     {
//         single_preset.increment();
//         single_preset.write_time(interface);
//     }
//     else if (item == og::menu::double_dose)
//     {
//         double_preset.increment();
//         double_preset.write_time(interface);
//     }
// }
// else if (interface.is_select_pressed())
// {
//     delay(300);
//
//     if (item == og::menu::purge)
//     {
//         perform_timer_loop(purge_preset, interface, motor);
//     }
//     else if (item == og::menu::single_dose)
//     {
//         perform_timer_loop(single_preset, interface, motor);
//     }
//     else if (item == og::menu::double_dose)
//     {
//         perform_timer_loop(double_preset, interface, motor);
//     }
//     else if (item == og::menu::settings)
//     {
//         display_settings_menu(interface);
//
//         item = og::menu::purge;
//         purge_preset.write(interface);
//     }
// }

// inline void display_settings_menu(lcd1602::display_keypad& interface)
// {
//     interface.display().setCursor(0, 0);
//     interface.display().print("dose weight");
//
//     enum class sub_menu_state : int {
//         purge_weight,
//         single_weight,
//         double_weight,
//         ground,
//         brightness,
//         back,
//         count
//     };
//
//     auto sub_menu = sub_menu_state::purge_weight;
//     purge_weight.write(interface);
//
//     while (true)
//     {
//         interface.update_button_state();
//
//         if (interface.is_none_pressed())
//         {
//             continue;
//         }
//
//         if (interface.is_select_pressed())
//         {
//             if (sub_menu == sub_menu_state::back)
//             {
//                 return;
//             }
//             else if (sub_menu == sub_menu_state::purge_weight)
//             {
//                 purge_weight.save();
//             }
//             else if (sub_menu == sub_menu_state::single_weight)
//             {
//                 single_weight.save();
//             }
//             else if (sub_menu == sub_menu_state::double_weight)
//             {
//                 double_weight.save();
//             }
//             interface.show_saved();
//         }
//         else if (interface.is_right_pressed())
//         {
//             if (sub_menu == sub_menu_state::purge_weight)
//             {
//                 purge_weight.increment();
//                 purge_weight.write(interface);
//             }
//             else if (sub_menu == sub_menu_state::single_weight)
//             {
//                 single_weight.increment();
//                 single_weight.write(interface);
//             }
//             else if (sub_menu == sub_menu_state::double_weight)
//             {
//                 double_weight.increment();
//                 double_weight.write(interface);
//             }
//             else if (sub_menu == sub_menu_state::brightness)
//             {
//                 interface.increase_brightness();
//                 interface.show_brightness_bar();
//             }
//         }
//         else if (interface.is_left_pressed())
//         {
//             if (sub_menu == sub_menu_state::purge_weight)
//             {
//                 purge_weight.decrement();
//                 purge_weight.write(interface);
//             }
//             else if (sub_menu == sub_menu_state::single_weight)
//             {
//                 single_weight.decrement();
//                 single_weight.write(interface);
//             }
//             else if (sub_menu == sub_menu_state::double_weight)
//             {
//                 double_weight.decrement();
//                 double_weight.write(interface);
//             }
//             else if (sub_menu == sub_menu_state::brightness)
//             {
//                 interface.decrease_brightness();
//                 interface.show_brightness_bar();
//             }
//         }
//         else if (interface.is_up_pressed())
//         {
//             if (sub_menu == sub_menu_state::single_weight)
//             {
//                 sub_menu = sub_menu_state::purge_weight;
//                 purge_weight.write(interface);
//             }
//             else if (sub_menu == sub_menu_state::double_weight)
//             {
//                 sub_menu = sub_menu_state::single_weight;
//                 single_weight.write(interface);
//             }
//             else if (sub_menu == sub_menu_state::brightness)
//             {
//                 sub_menu = sub_menu_state::double_weight;
//                 double_weight.write(interface);
//             }
//             else if (sub_menu == sub_menu_state::back)
//             {
//                 sub_menu = sub_menu_state::brightness;
//                 interface.show_brightness_bar();
//             }
//         }
//         else if (interface.is_down_pressed())
//         {
//             if (sub_menu == sub_menu_state::purge_weight)
//             {
//                 sub_menu = sub_menu_state::single_weight;
//                 single_weight.write(interface);
//             }
//             else if (sub_menu == sub_menu_state::single_weight)
//             {
//                 sub_menu = sub_menu_state::double_weight;
//                 double_weight.write(interface);
//             }
//             else if (sub_menu == sub_menu_state::double_weight)
//             {
//                 sub_menu = sub_menu_state::brightness;
//                 interface.show_brightness_bar();
//             }
//             else if (sub_menu == sub_menu_state::brightness)
//             {
//                 sub_menu = sub_menu_state::back;
//                 interface.display().clear();
//                 interface.display().print("return");
//             }
//         }
//         delay(300);
//     }
// }
