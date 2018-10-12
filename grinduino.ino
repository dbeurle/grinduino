
#include <EEPROM.h>
#include <LiquidCrystal.h>

namespace lcd1602
{
class display_keypad
{
public:
    enum class button : int8_t { right, up, down, left, select, none, unknown };

public:
    display_keypad() : m_lcd(8, 9, 4, 5, 6, 7) { m_lcd.begin(16, 2); }

    auto display() noexcept -> LiquidCrystal& { return m_lcd; }

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

    bool is_select_pressed() const noexcept { return m_current == button::select; }
    bool is_left_pressed() const noexcept { return m_current == button::left; }
    bool is_right_pressed() const noexcept { return m_current == button::right; }
    bool is_up_pressed() const noexcept { return m_current == button::up; }
    bool is_down_pressed() const noexcept { return m_current == button::down; }
    bool is_none_pressed() const noexcept { return m_current == button::none; }

    bool has_state_changed() const noexcept { return m_has_state_changed; }

private:
    auto convert_to_code(int const analog_key_input) const noexcept -> button
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
        else if (analog_key_input > 50 && analog_key_input < 150)
        {
            return button::up;
        }
        else if (analog_key_input >= 150 && analog_key_input < 350)
        {
            return button::down;
        }
        else if (analog_key_input > 350 && analog_key_input < 500)
        {
            return button::left;
        }
        return button::select;
    }

private:
    LiquidCrystal m_lcd;

    /// Current known state of the switch
    button m_current = button::none;
    button m_last = button::none;

    bool m_has_state_changed = false;

    unsigned long m_last_debounce = 0;
};
}
namespace grinduino
{
class timer_preset
{
public:
    timer_preset(char const* name, int const eeprom_index, unsigned long const time_ms = 1000)
        : m_name(name), m_time(time_ms), m_eeprom_index(eeprom_index)
    {
    }

    void set(unsigned int const time_value) noexcept { m_time = time_value; }

    auto value() const noexcept -> unsigned int { return m_time; }

    /// Increment the timer by 100 ms
    void increment() { m_time += 100; }
    /// Decrement the timer by 100 ms
    void decrement() { m_time = (m_time >= 100) ? m_time - 100 : 0; }

    /// Load a value from the EEPROM of the device \sa save()
    void load() noexcept { m_time = EEPROM.read(m_eeprom_index) * 100; }
    /// Load a value from the EEPROM of the device \sa write()
    void save() noexcept { EEPROM.write(m_eeprom_index, m_time / 100); }

    /// Write a value into the EEPROM of the device
    void write(lcd1602::display_keypad& interface) const noexcept
    {
        interface.display().clear();
        interface.display().setCursor(0, 0);
        interface.display().print(m_name);

        interface.display().setCursor(0, 1);
        interface.display().print("duration = ");

        this->print_duration(interface, m_time);
    }

    /// Write a value into the EEPROM of the device
    void write_time(lcd1602::display_keypad& interface) const noexcept
    {
        this->print_duration(interface, m_time);
    }

    void start() noexcept { m_started_at = millis(); }

    void reset() noexcept { m_started_at = 0; }

    bool is_finished() const { return millis() - m_started_at > m_time; }

    void countdown(lcd1602::display_keypad& interface)
    {
        this->print_duration(interface, m_time - (millis() - m_started_at));
    }

private:
    void print_duration(lcd1602::display_keypad& interface, unsigned long const time_ms) const
    {
        if (time_ms < 10000)
        {
            interface.display().setCursor(15, 1);
            interface.display().print(" ");
        }

        interface.display().setCursor(11, 1);
        interface.display().print(time_ms / 1000.0f, 1);
        interface.display().print("s");
    }

private:
    char const* m_name;
    unsigned long m_time = 0;
    unsigned long m_started_at = 0;
    int m_eeprom_index;
};

enum class menu : int { home, purge, single_dose, double_dose, settings, count };
/// motor is responsible for interfacing the with SSR that control the motor
/// energisation state (on / off)
class motor
{
public:
    motor(int const pin = LED_BUILTIN) : m_pin(pin)
    {
        pinMode(m_pin, OUTPUT);
        digitalWrite(m_pin, LOW);
    }

    void on() noexcept
    {
        m_state = true;
        digitalWrite(m_pin, HIGH);
    }

    void off() noexcept
    {
        m_state = false;
        digitalWrite(m_pin, LOW);
    }

    bool state() const noexcept { return m_state; }

private:
    bool m_state = false;
    int m_pin;
};
}

lcd1602::display_keypad interface;

grinduino::timer_preset purge_preset("purge", 0);
grinduino::timer_preset single_preset("single dose", 1);
grinduino::timer_preset double_preset("double dose", 2);

grinduino::motor motor(13);

grinduino::menu item = grinduino::menu::purge;

/// Print a cute welcome message
inline void welcome() noexcept
{
    interface.display().clear();
    interface.display().setCursor(4, 0);
    interface.display().print("grinduino");
    interface.display().setCursor(2, 1);
    interface.display().print("version 0.1");
    delay(1000);
}

/// Perform the timer loop withou blocking the button update.  This enables the
/// select button presses to be recognised
inline void perform_timer_loop(grinduino::timer_preset& timer,
                               lcd1602::display_keypad& interface,
                               grinduino::motor& motor)
{
    timer.save();
    timer.start();

    motor.on();

    while (!timer.is_finished())
    {
        interface.update_button_state();

        // Break out of the countdown if user aborted
        if (interface.is_select_pressed())
        {
            break;
        }
        timer.countdown(interface);
    }
    motor.off();
    timer.reset();

    timer.write(interface);
}

void setup()
{
    interface.display().begin(16, 2);

    welcome();

    purge_preset.load();
    single_preset.load();
    double_preset.load();

    purge_preset.write(interface);
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
        if (item == grinduino::menu::purge)
        {
            item = grinduino::menu::single_dose;
            single_preset.write(interface);
        }
        else if (item == grinduino::menu::single_dose)
        {
            item = grinduino::menu::double_dose;
            double_preset.write(interface);
        }
        else if (item == grinduino::menu::double_dose)
        {
            item = grinduino::menu::settings;

            interface.display().clear();
            interface.display().setCursor(0, 0);
            interface.display().print("settings");
        }
    }
    else if (interface.is_up_pressed())
    {
        if (item == grinduino::menu::single_dose)
        {
            item = grinduino::menu::purge;
            purge_preset.write(interface);
        }
        else if (item == grinduino::menu::double_dose)
        {
            item = grinduino::menu::single_dose;
            single_preset.write(interface);
        }
        else if (item == grinduino::menu::settings)
        {
            item = grinduino::menu::double_dose;
            double_preset.write(interface);
        }
    }
    else if (interface.is_left_pressed())
    {
        if (item == grinduino::menu::purge)
        {
            purge_preset.decrement();
            purge_preset.write_time(interface);
        }
        else if (item == grinduino::menu::single_dose)
        {
            single_preset.decrement();
            single_preset.write_time(interface);
        }
        else if (item == grinduino::menu::double_dose)
        {
            double_preset.decrement();
            double_preset.write_time(interface);
        }
    }
    else if (interface.is_right_pressed())
    {
        if (item == grinduino::menu::purge)
        {
            purge_preset.increment();
            purge_preset.write_time(interface);
        }
        else if (item == grinduino::menu::single_dose)
        {
            single_preset.increment();
            single_preset.write_time(interface);
        }
        else if (item == grinduino::menu::double_dose)
        {
            double_preset.increment();
            double_preset.write_time(interface);
        }
    }
    else if (interface.is_select_pressed())
    {
        delay(300);

        if (item == grinduino::menu::purge)
        {
            perform_timer_loop(purge_preset, interface, motor);
        }
        else if (item == grinduino::menu::single_dose)
        {
            perform_timer_loop(single_preset, interface, motor);
        }
        else if (item == grinduino::menu::double_dose)
        {
            perform_timer_loop(double_preset, interface, motor);
        }
    }
    delay(200);
}
