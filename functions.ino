
// check button positions and write values pertaining to their states
void buttonCheck()
{
    upVal = digitalRead(up);
    downVal = digitalRead(down);
    selectVal = digitalRead(select); // reads the pin state and prints it as a val
    activeVal = digitalRead(active);
    purgeVal = digitalRead(purge);

    if (upVal == LOW && downVal == HIGH && selectVal == HIGH)
    {
        // when the upbutton is pressed
        upState = 1;   // the upState is 1 while downState is 0
        downState = 0; //
    }
    else if (upVal == HIGH && downVal == LOW && selectVal == HIGH)
    {                  // when the down button is pressed
        upState = 0;   //
        downState = 1; // the downState is 1 while the upState is 0
    }
    else if (upVal == HIGH && downVal == HIGH && selectVal == LOW)
    {
        // when the select button is pressed wait 0.2 seconds (debounce)
        delay(200);
        // the selectState changes state from 0 to 1 or 1 to 0.
        selectState = !selectState;
    }
    else if ((activeVal == LOW) && (purgeVal == HIGH))
    {
        activeState = 1;
        purgeState = 0;
    }
    else if ((activeVal == HIGH) && (purgeVal == LOW))
    {
        activeState = 0;
        purgeState = 1;
    }
    else
    {
        // in any other conditions the up, down, active and purge states are 0
        downState = upState = activeState = purgeState = 0;
    }
}

// displays the preset 1 information
void presetOneLCD()
{
    lcd.clear();                    // clears previous info on LCD screen
    lcd.setCursor(4, 0);            // sets cursor to column 4, row 0.
    lcd.print("PRESET 1");          // prints "preset 1"
    lcd.setCursor(0, 1);            // sets cursor to column 0, row 1.
    lcd.print("DURATION = ");       // prints "duration = "
    lcd.print(presetOneVal / 10.0); // prints the preset one time value.
}

// creates a function to display the setting of preset 1
void presetOneSelect()
{
    lcd.clear();                    // clears previous info on LCD screen
    lcd.setCursor(2, 0);            // sets cursor to column 2, row 0.
    lcd.print("SET PRESET 1");      // prints "set preset 1"
    lcd.setCursor(0, 1);            // sets cursor to column 0, row 1.
    lcd.print("DURATION = ");       // prints "duration = "
    lcd.print(presetOneVal / 10.0); // prints the preset one time value.
}

// displays the preset 2 information
void presetTwoLCD()
{
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("PRESET 2");
    lcd.setCursor(0, 1);
    lcd.print("DURATION = ");
    lcd.print(presetTwoVal / 10.0);
}

// creates a function to display the setting of preset 2
void presetTwoSelect()
{
    lcd.clear();                    // clears previous info on LCD screen
    lcd.setCursor(2, 0);            // sets cursor to column 2, row 0.
    lcd.print("SET PRESET 2");      // prints "set preset 1"
    lcd.setCursor(0, 1);            // sets cursor to column 0, row 1.
    lcd.print("DURATION = ");       // prints "duration = "
    lcd.print(presetTwoVal / 10.0); // prints the preset one time value.
}

// creates a function to display information on preset 1 and 2 while up and down are pressed
void presetFunctions()
{
    if (upState == 1 && selectState == 0)
    {
        // when up is pressed and select is not engaged then lcdState is 1.
        lcdState = 1;
    }
    else if (downState == 1 && selectState == 0)
    {
        // when down is pressed and select is not egnaged then lcdState is 0.
        lcdState = 0;
    }

    if (lcdState == 0 && oldlcdState == 1)
    {
        // when down has been pressed once and select has not been pressed then
        presetOneLCD();  // show preset 1 information on the lcd.
        oldlcdState = 0; // make old lcd state 0 so the display is not repeated.
    }
    else if (lcdState == 1 && oldlcdState == 0)
    {
        // when up has been pressed once and select has not been pressed then
        presetTwoLCD();  // show preset 2 information on the lcd
        oldlcdState = 1; // make old lcd state 1 so the display is not repeated.
    }

    // for SELECT stage on preset 1
    while (lcdState == 0 && selectState == 1)
    {
        // when lcd State is 0 (showing preset 1 info) and select has been engaged then
        presetOneSelect(); // display setting preset one info on LCD
        delay(100);        // delay 0.1 seconds to decrease amount flashed to LCD
        oldlcdState = 1; // make oldlcdstate = 1 so that it will return to display preset 1 info once select is pressed
        buttonCheck(); // check what buttons have been pressed (to know if select has been pressed
                       // so it can turn off, or if up or down have been pressed
        if (upState == 1)
        {
            // if up button is pressed then
            // add 0.1 seconds to the preset 1 time and clamp between 0.1 and 25.5
            presetOneVal = constrain(++presetOneVal, 1, 255);
        }
        else if (downState == 1)
        {
            // if the down button is pressed then minus 0.1 seconds to the preset 1 time
            // limit the value range to be 0.1 to 25.5
            presetOneVal = constrain(--presetOneVal, 1, 255);
        }
        oldSelectState = 1; // make oldSelectState = 1 so we can write the value to memory later.
    }

    // for SELECT stage on preset 2
    while (lcdState == 1 && selectState == 1)
    {
        // when lcd State is 1 (showing preset 2 info) and select has been engaged then
        presetTwoSelect(); // display setting preset 2 info on LCD
        delay(100);        // delay 0.1 seconds to decrease amount flashed to LCD
        oldlcdState = 0; // make oldlcdstate = 0 so that it will return to display preset 1 info once select button is pressed
        buttonCheck();   // check what buttons have been pressed (to know if select has been pressed
                         // so it can turn off, or if up or down have been pressed
        if (upState == 1)
        {                                                   // if up button is pressed then
            presetTwoVal = presetTwoVal + 1;                // add 0.1 seconds to the preset 2 time
            presetTwoVal = constrain(presetTwoVal, 1, 255); // limit the value range to be 0.1 to 25.5
        }
        else if (downState == 1)
        {                                    // if the down button is pressed then
            presetTwoVal = presetTwoVal - 1; // minus 0.1 seconds to the preset 2 time
            presetTwoVal = constrain(presetTwoVal, 1, 255); // limit the value range to be 0.1 to 25.5
        }
        oldSelectState = 1; // make oldSelectState = 1 so we can write the value to memory later.
    }
    if (lcdState == 0 && selectState == 0 && oldSelectState == 1)
    {
        // when preset 1 info is displayed, after select has changed from state 1 to state 0 then
        oldSelectState = 0; // old select state changes to 0 (so it doesn't repeat)
        spitPresetOne();    // write the preset one value to the eeprom memory
    }
    else if (lcdState == 1 && selectState == 0 && oldSelectState == 1)
    {
        // when preset 2 info is displayed, after select has changed from state 1 to state 0 then
        oldSelectState = 0; // old select state changes to 0 (so it doesn't repeat)
        spitPresetTwo();    // write the preset two value to the eeprom memory
    }

    // ACTIVE BUTTON FUNCTIONS
    while (lcdState == 0 && activeState == 1)
    {
        // when preset 1 is shown and the active button is pressed
        while (x < 1)
        {
            // sets up a single use function
            for (x = 0; x < 1; x++)
            {                                 // do the next 5 lines, once
                lcd.clear();                  // clear the LCD
                lcd.setCursor(0, 0);          // set the cursor to the top left
                lcd.print("PRESET 1 ACTIVE"); // write "preset 1 active"
                lcd.setCursor(0, 1);          // set the cursor to the bottom left
                lcd.print("REMAINING = ");    // write "remaining ="
            }
        }
        previousMillis = currentMillis; // save the the millis() as previousMillis as the button has been pressed
        pOneMillis = presetOneVal * 100; // convert presetOneVal from its EEPROM friendly form
                                         // into a milliSecond friendly value

        // create the milliSecond value that this while loop should stop at
        pOneMillisLength = previousMillis + pOneMillis;

        countDown = pOneMillisLength - millis(); // countDown is the difference between the actual
                                                 // millis() value and pOneMillisLength

        dig1 = countDown / 1000;                    // dig1 is the first digit of countdown
        dig2 = ((countDown - (dig1 * 1000)) / 100); // dig2 is the second digit of countdown
        lcd.setCursor(12, 1);                       // move the lcd Cursor to 12,1
        lcd.print(dig1);                            // print the value of dig1
        lcd.print(".");                             // print a decimal point
        lcd.print(dig2);                            // print the value of dig2

        if (millis() < pOneMillisLength)
        {
            // when millis is less than the milliSecond value that this while loop should stop at
            digitalWrite(optoOne, HIGH);
        }
        else
        {
            // when millis is greater that the millisecond value that this while loop should stop at
            digitalWrite(optoOne, LOW);
            presetOneLCD();  // return to the preset one display
            activeState = 0; // make the activeState = 0 so the while loop stops
            x = 0;           // return x to 0 so that the above "for" loop can be repeated
        }
    }

    while (lcdState == 1 && activeState == 1)
    {
        // when preset 2 is shown and the active button is pressed
        while (x < 1)
        {
            // sets up a single use function
            for (x = 0; x < 1; x++)
            {                                 // do the next 5 lines, once
                lcd.clear();                  // clear the LCD
                lcd.setCursor(0, 0);          // set the cursor to the top left
                lcd.print("PRESET 2 ACTIVE"); // write "preset 2 active"
                lcd.setCursor(0, 1);          // set the cursor to the bottom left
                lcd.print("REMAINING = ");    // write "remaining ="
            }
        }
        previousMillis = currentMillis; // save the the millis() as previousMillis as the button has been pressed
        pTwoMillis = (presetTwoVal * 100); // convert presetTwoVal from its EEPROM friendly form
                                           // into a milliSecond friendly value

        // create the milliSecond value that this while loop should stop at
        pTwoMillisLength = (previousMillis + pTwoMillis);

        countDown = pTwoMillisLength - millis(); // countDown is the difference between the actual
                                                 // millis() value and pTwoMillisLength

        dig1 = countDown / 1000;                    // dig1 is the first digit of countdown
        dig2 = ((countDown - (dig1 * 1000)) / 100); // dig2 is the second digit of countdown
        lcd.setCursor(12, 1);                       // move the lcd Cursor to 12,1
        lcd.print(dig1);                            // print the value of dig1
        lcd.print(".");                             // print a decimal point
        lcd.print(dig2);                            // print the value of dig2

        if (millis() < pTwoMillisLength)
        {
            // when millis is less than the milliSecond value that this while loop should stop at
            digitalWrite(optoOne, HIGH);
        }
        else
        {
            // when millis is greater that the millisecond value that this while loop should stop at
            digitalWrite(optoOne, LOW); //
            presetTwoLCD();             // return to the preset one display
            activeState = 0;            // make the activeState = 0 so the while loop stops
            x = 0; // return x to 0 so that the above "for" loop can be repeated
        }
    }

    while (purgeState == 1)
    {
        digitalWrite(optoOne, HIGH);
        buttonCheck();
        if (purgeState == 0)
        {
            digitalWrite(optoOne, LOW);
        }
    }
}

// Function to read preset values from EEPROM (needs only be done once during startup)
void sniffPresets()
{
    presetOneVal = EEPROM.read(0); // read the preset one integer value from EEPROM
    presetTwoVal = EEPROM.read(1); // read the preset two integer value from EEPROM
}

// Functions to write the preset values to the EEPROM (needs to be done ONCE after each
// change is made to the value)
void spitPresetOne()
{
    EEPROM.write(0, presetOneVal); // write the integer into the eeprom
}

void spitPresetTwo()
{
    EEPROM.write(1, presetTwoVal); // write the integer into the eeprom
}

void welcome()
{
    lcd.clear();
    lcd.home();
    // On ya Ian
    lcd.print("IAN IS A");
    lcd.setCursor(0, 1);
    lcd.print("DIRTY DILDO!");
}
