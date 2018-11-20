
#include "persistent_storage.hpp"
#include "display_keypad.hpp"
#include "main_menu.hpp"
#include "solid_state_relay.hpp"
#include "dose_weight.hpp"

lcd1602::display_keypad interface;

og::dose_weight purge_weight(og::persistent_storage{}.allocate(), "Purge");
og::dose_weight single_weight(og::persistent_storage{}.allocate(), "Single Dose");
og::dose_weight double_weight(og::persistent_storage{}.allocate(), "Double Dose");

og::solid_state_relay motor;

/// Print a cute welcome message
static void welcome()
{
    interface.display().clear();
    interface.display().setCursor(2, 0);
    interface.display().print(F("Open Grinder"));
    interface.display().setCursor(2, 1);
    interface.display().print(F("Version 0.0.2"));
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
