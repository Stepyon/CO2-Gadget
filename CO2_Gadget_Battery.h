/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                             SETUP BATTERY FUNCTIONALITY                           *********/
/*********                                                                                   *********/
/*****************************************************************************************************/

float battery_voltage = 0;

void readBatteryVoltage()
{
  static uint64_t timeStamp = 0;
  if (millis() - timeStamp > 60000) { // Read battery voltage one time each minute
    timeStamp = millis();
    uint16_t v = analogRead(ADC_PIN);
    battery_voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);
    Serial.printf(" Battery voltage: %.2f\n", battery_voltage);
  }
}