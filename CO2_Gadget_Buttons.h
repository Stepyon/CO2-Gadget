#ifndef CO2_Gadget_Buttons_h
#define CO2_Gadget_Buttons_h

#include "Button2.h"
#undef LONGCLICK_TIME_MS
#define LONGCLICK_TIME_MS 300  // https://github.com/LennartHennigs/Button2/issues/10
Button2 btnUp(BTN_UP);         // Initialize the up button
Button2 btnDwn(BTN_DWN);       // Initialize the down button

void IRAM_ATTR buttonUpISR() {
    if (actualDisplayBrightness == 0) {  // Turn on the display only if it's OFF
        shouldWakeUpDisplay = true;
        lastTimeButtonPressed = millis();
    }
    if (!menuInitialized) {  // If the menu is not initialized yet, initialize it now (disable Improv WiFi functionality)
        mustInitMenu = true;
        publishMQTTLogData("-->[BUTT] mustInitMenu: " + String(mustInitMenu));
    }
}

void IRAM_ATTR buttonDownISR() {
    if (actualDisplayBrightness == 0) {  // Turn on the display only if it's OFF
        shouldWakeUpDisplay = true;
        lastTimeButtonPressed = millis();
    }
    if (!menuInitialized) {  // If the menu is not initialized yet, initialize it now (disable Improv WiFi functionality)
        mustInitMenu = true;
        publishMQTTLogData("-->[BUTT] mustInitMenu: " + String(mustInitMenu));
    }
}

// void doubleClick(Button2& btn) {
//   Serial.println("-->[BUTT] Test double click...");
//   displayNotification("Test functionality", "double click", notifyInfo);
// }

void initButtons() {
    // Interrupt Service Routine to turn on the display on button UP press
    if (BTN_UP >= 0) {
        attachInterrupt(BTN_UP, buttonUpISR, RISING);
        btnUp.setLongClickTime(LONGCLICK_TIME_MS);
        btnUp.setLongClickHandler([](Button2 &b) { nav.doNav(enterCmd); });
        btnUp.setClickHandler([](Button2 &b) {
            // Up
            nav.doNav(downCmd);
        });
    }
    if (BTN_DWN >= 0) {
        // attachInterrupt(BTN_DWN, buttonUpISR, RISING); // Conflicts with Improv because of GPIO 0
        btnDwn.setLongClickTime(LONGCLICK_TIME_MS);
        btnDwn.setLongClickHandler([](Button2 &b) { nav.doNav(escCmd); });
        btnDwn.setClickHandler([](Button2 &b) {
            // Down
            nav.doNav(upCmd);
        });
    }

    // btnDwn.setDoubleClickHandler(doubleClick);
}

void reverseButtons(bool reversed) {
    if (reversed) {
        // Interrupt Service Routine to turn on the display on button UP press
        if ((BTN_UP >= 0) && (BTN_DWN >= 0)) {
            attachInterrupt(BTN_UP, buttonUpISR, RISING);
            btnDwn.setLongClickTime(LONGCLICK_TIME_MS);
            btnDwn.setLongClickHandler([](Button2 &b) { nav.doNav(enterCmd); });
            btnDwn.setClickHandler([](Button2 &b) {
                // Up
                nav.doNav(downCmd);
            });

            btnUp.setLongClickTime(LONGCLICK_TIME_MS);
            btnUp.setLongClickHandler([](Button2 &b) { nav.doNav(escCmd); });
            btnUp.setClickHandler([](Button2 &b) {
                // Down
                nav.doNav(upCmd);
            });
        }
    } else {
        if ((BTN_UP >= 0) && (BTN_DWN >= 0)) {
            attachInterrupt(BTN_DWN, buttonUpISR, RISING);
            btnUp.setLongClickTime(LONGCLICK_TIME_MS);
            btnUp.setLongClickHandler([](Button2 &b) { nav.doNav(enterCmd); });
            btnUp.setClickHandler([](Button2 &b) {
                // Up
                nav.doNav(downCmd);
            });

            btnDwn.setLongClickTime(LONGCLICK_TIME_MS);
            btnDwn.setLongClickHandler([](Button2 &b) { nav.doNav(escCmd); });
            btnDwn.setClickHandler([](Button2 &b) {
                // Down
                nav.doNav(upCmd);
            });
        }
    }
}

void buttonsLoop() {
    // Check for button presses
    if (BTN_UP >= 0) {
        btnUp.loop();
    }
    if (BTN_DWN >= 0) {
        btnDwn.loop();
    }
}

bool isButtonPressedOnWakeUp() {
    unsigned long buttonPressStartTime = millis();
    bool buttonState = digitalRead(BTN_DWN);  // Initial button state

    // If the button is not pressed, return false immediately
    if (buttonState == HIGH) {
        return false;
    }

    // Wait for the button to be released or for LONGCLICK_TIME_MS
    while (buttonState == LOW && (millis() - buttonPressStartTime < LONGCLICK_TIME_MS)) {
        delay(10);  // Debouncing delay
        buttonState = digitalRead(BTN_DWN);

        // Check if the button is released
        if (buttonState == HIGH) {
            // Button was released before LONGCLICK_TIME_MS
            return false;
        }
    }

    // Check if the button is still pressed after waiting
    if (buttonState == LOW && (millis() - buttonPressStartTime >= LONGCLICK_TIME_MS)) {
        // Button was pressed and held for LONGCLICK_TIME_MS
        return true;
    }

    return false;
}

#endif  // CO2_Gadget_Buttons_h