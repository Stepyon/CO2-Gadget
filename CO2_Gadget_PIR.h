void initPIR() {
    pinMode(PIR_PIN, INPUT);
}
void PIRLoop() {
    if(displayOnByPIRSensor && digitalRead(PIR_PIN) == HIGH) {
        shouldWakeUpDisplay = true;
        lastTimeButtonPressed = millis();
    }
}