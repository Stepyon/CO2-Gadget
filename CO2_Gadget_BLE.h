#ifndef CO2_Gadget_BLE_h
#define CO2_Gadget_BLE_h

#ifdef SUPPORT_BLE
#include "Sensirion_Gadget_BLE.h"
#include "WifiMultiLibraryWrapper.h"

// clang-format on

NimBLELibraryWrapper lib;
WifiMultiLibraryWrapper wifi;
DataProvider provider(lib, DataType::T_RH_CO2, true, true, true, &wifi);
#endif

void setBLEHistoryInterval(uint64_t interval) {
#ifdef SUPPORT_BLE
    if (provider.getHistoryInterval() != interval * 1000) {
#ifdef DEBUG_BLE
        Serial.flush();
        Serial.println("-->[BLE ] Actual history interval: " + String(provider.getHistoryInterval() / 1000) + " seconds");
        Serial.println("-->[BLE ] Setting history interval to: " + String(interval) + " seconds");
        delay(20);
#endif
        provider.setHistoryInterval(interval * 1000);
    }
#endif
}

void initBLE() {
#ifdef SUPPORT_BLE
    if (activeBLE) {
        if (bleInitialized) {
            Serial.print(
                "-->[BLE ] Sensirion Gadget BLE Lib already initialized with deviceId = ");
            Serial.println(provider.getDeviceIdString());
            return;
        } else {
            setBLEHistoryInterval(sampleInterval);
            provider.begin();
            Serial.print("-->[BLE ] Sensirion Gadget BLE Lib initialized with deviceId = ");
            Serial.println(provider.getDeviceIdString());
            Serial.print("-->[BLE ] History interval set to: ");
            Serial.print(provider.getHistoryInterval() / 1000);
            Serial.println(" seconds");
            bleInitialized = true;
        }
    }
#endif
}

/**
 * @brief Publishes sensor data over BLE (Bluetooth Low Energy).
 *
 * This function is responsible for publishing sensor data over BLE if the BLE support is enabled and the sensor readings are within the valid range.
 * The sensor data includes CO2 level, temperature, and humidity.
 *
 * @note This function should be called periodically to publish the sensor data.
 */
void publishBLE() {
    static int64_t lastMeasurementTimeMs = 0;
    static int measurementIntervalMs = 1000;
#ifdef SUPPORT_BLE
    if (isDownloadingBLE) {
        return;
    }
    if (millis() - lastMeasurementTimeMs >= measurementIntervalMs) {
        if ((activeBLE) && (co2 >= 400) && (co2 <= 5000) && (temp >= -40) && (temp <= 85) && (hum >= 0) && (hum <= 100)) {
            provider.writeValueToCurrentSample(co2, SignalType::CO2_PARTS_PER_MILLION);
            provider.writeValueToCurrentSample(temp, SignalType::TEMPERATURE_DEGREES_CELSIUS);
            provider.writeValueToCurrentSample(hum, SignalType::RELATIVE_HUMIDITY_PERCENTAGE);
            provider.commitSample();
            lastMeasurementTimeMs = millis();
        }
#ifdef DEBUG_BLE
        Serial.println("-->[BLE ] Sent CO2: " + String(co2) + " ppm, Temp: " + String(temp) + " C, Hum: " + String(hum) + " %");
        publishMQTTLogData("-->[BLE ] Sent CO2: " + String(co2) + " ppm, Temp: " + String(temp) + " C, Hum: " + String(hum) + " %");
        delay(20);
#endif
    }
#endif
}

void handleBLEwifiChanged() {
#ifdef SUPPORT_BLE
    wifiSSID = provider.getWifiSSID();
    wifiPass = provider.getWifiPassword();
    wifiSSID.trim();
    wifiPass.trim();
    wifiChanged = true;
    activeWIFI = true;
    Serial.println("-->[BLE ] Wifi SSID changed to: #" + wifiSSID + "#");
#ifndef WIFI_PRIVACY
    Serial.println("-->[BLE ] Wifi password changed to: #" + wifiPass + "#");
#endif
    delay(5);
#endif
}

void handleFrcRequest() {
#ifdef SUPPORT_BLE
    if (!provider.isFRCRequested()) {
        return;
    }

    calibrationValue = provider.getReferenceCO2Level();
    pendingCalibration = true;

    Serial.print("[BLE ] Received FRC request (calibration) with reference CO2 level: ");
    Serial.println(calibrationValue);
    delay(5);
    provider.completeFRCRequest();
#endif
}

void BLELoop() {
#ifdef SUPPORT_BLE
    int connectTries = 0;
    if (!activeBLE) {
        return;
    }
    provider.handleDownload();
    isDownloadingBLE = provider.isDownloading();
    if (isDownloadingBLE) return;
    // delay(3);
    if (provider.wifiChanged()) handleBLEwifiChanged();
    if (provider.historyIntervalChanged()) {
        Serial.print("-->[BLE ] History interval changed to: ");
        Serial.print(provider.getHistoryInterval() / 1000);
        Serial.println(" seconds");
    }
    handleFrcRequest();
#endif
}

#endif  // CO2_Gadget_BLE_h