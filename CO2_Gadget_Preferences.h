#ifndef CO2_Gadget_Preferences_h
#define CO2_Gadget_Preferences_h

#include <ArduinoJson.h>
#include <Preferences.h>
Preferences preferences;

uint8_t prefVersion = 0;
uint8_t prefRevision = 0;
uint8_t firmVersionMajor = 0;
uint8_t firmVersionMinor = 0;
uint8_t firmRevision = 0;
String firmBranch = "";
String firmFlavour = "";

// Function to extract the major version number as an integer
int getCO2GadgetMajorVersion() {
#ifdef CO2_GADGET_VERSION
    // Find the position of the first dot
    int dotIndex = 0;
    while (CO2_GADGET_VERSION[dotIndex] != '.' && CO2_GADGET_VERSION[dotIndex] != '\0') {
        dotIndex++;
    }
    // Create a temporary buffer to hold the major version substring
    char majorVersion[dotIndex + 1];
    // Copy the characters until the dot into the buffer
    strncpy(majorVersion, CO2_GADGET_VERSION, dotIndex);
    // Null-terminate the string
    majorVersion[dotIndex] = '\0';
#ifdef DEBUG_PREFERENCES
    Serial.println("-->[PREF] Major version: " + String(majorVersion));
#endif
    // Convert the substring to an integer
    return atoi(majorVersion);
#else
    return false;
#endif
}

// Function to extract the minor version number as an integer
int getCO2GadgetMinorVersion() {
#ifdef CO2_GADGET_VERSION
    // Find the position of the first dot
    int dotIndex = 0;
    while (CO2_GADGET_VERSION[dotIndex] != '.' && CO2_GADGET_VERSION[dotIndex] != '\0') {
        dotIndex++;
    }
    // Check if a dot was found
    if (CO2_GADGET_VERSION[dotIndex] == '\0') {
#ifdef DEBUG_PREFERENCES
        Serial.println("-->[PREF] Error: Minor version not found.");
#endif
        return -1;  // Sentinel value indicating error
    }
    // Find the position of the second dot
    int minorIndex = dotIndex + 1;
    while (CO2_GADGET_VERSION[minorIndex] != '.' && CO2_GADGET_VERSION[minorIndex] != '\0') {
        minorIndex++;
    }
    // Check if a second dot was found
    if (CO2_GADGET_VERSION[minorIndex] == '\0') {
#ifdef DEBUG_PREFERENCES
        Serial.println("-->[PREF] Error: Minor version format error.");
#endif
        return -1;  // Sentinel value indicating error
    }
    // Extract the substring between the first and second dots and convert it to an integer
    char minorVersionStr[minorIndex - dotIndex];
    strncpy(minorVersionStr, CO2_GADGET_VERSION + dotIndex + 1, minorIndex - dotIndex - 1);
    minorVersionStr[minorIndex - dotIndex - 1] = '\0';  // Null-terminate the string
#ifdef DEBUG_PREFERENCES
    Serial.println("-->[PREF] Minor version: " + String(minorVersionStr));
#endif
    return atoi(minorVersionStr);
#else
    return -1;  // Sentinel value indicating error
#endif
}

// Function to extract the revision number as an integer
int getCO2GadgetRevisionNumber() {
#ifdef CO2_GADGET_REV
#ifdef DEBUG_PREFERENCES
    Serial.println("-->[PREF] CO2 Gadget Revision: " + String(CO2_GADGET_REV));
#endif
    // Find the position of the dash
    int dashIndex = 0;
    while (CO2_GADGET_REV[dashIndex] != '-' && CO2_GADGET_REV[dashIndex] != '\0') {
        if (!isdigit(CO2_GADGET_REV[dashIndex])) {
            // If the character is not a digit, return error
            return -1;
        }
        dashIndex++;
    }
#ifdef DEBUG_PREFERENCES
    Serial.println("-->[PREF] Dash index: " + String(dashIndex));
#endif
    // Extract the substring from the beginning to the dash
    char revNumberStr[dashIndex + 1];  // Add 1 for the null terminator
    strncpy(revNumberStr, CO2_GADGET_REV, dashIndex);
    revNumberStr[dashIndex] = '\0';  // Null-terminate the string
#ifdef DEBUG_PREFERENCES
    // Print the revision number to the serial console
    Serial.print("-->[PREF] Revision number: ");
    Serial.println(revNumberStr);
#endif
    return atoi(revNumberStr);
#else
    return -1;  // Sentinel value indicating error
#endif
}

// Function to extract the revision branch as a String
String getCO2GadgetRevisionBranch() {
#ifdef CO2_GADGET_REV
    // Find the position of the first dash
    int dashIndex = 0;
    // Search for the dash character '-'
    while (CO2_GADGET_REV[dashIndex] != '-' && CO2_GADGET_REV[dashIndex] != '\0') {
        dashIndex++;
    }
    // If a dash is found, extract the substring after the dash
    if (CO2_GADGET_REV[dashIndex] == '-') {
        int branchIndex = dashIndex + 1;
        // Create a new String to hold the branch name
        String branchName = "";
        // Copy characters after the dash to the branchName string
        while (CO2_GADGET_REV[branchIndex] != '\0') {
            branchName += CO2_GADGET_REV[branchIndex];
            branchIndex++;
        }
#ifdef DEBUG_PREFERENCES
        Serial.println("-->[PREF] Revision branch: " + branchName);
#endif
        return branchName;
    } else {
        // If no dash is found, return an empty string
        return "";
    }
#else
    // If CO2_GADGET_REV is not defined, return an empty string
    return "";
#endif
}

void upgradePreferences() {
    if ((batteryDischargedMillivolts == 3500) && (prefVersion == 0) && (prefRevision == 0)) {
        batteryDischargedMillivolts = 3200;
        Serial.println("-->[PREF] Upgrading preferences (" + String(__func__) + ") batteryDischargedMillivolts to: " + String(batteryDischargedMillivolts));
        prefRevision = 1;
        putPreferences();
    }
}

void printActualSettings() {
    Serial.println("-->[PREF]");
    Serial.println("-->[PREF] LOADED PREFERENCES FROM NVR:");
    Serial.println("-->[PREF] prefVersion:\t #" + String(prefVersion) + "#");
    Serial.println("-->[PREF] prefRevision:\t #" + String(prefRevision) + "#");
    Serial.println("-->[PREF] firmVersionMajor:\t #" + String(firmVersionMajor) + "#");
    Serial.println("-->[PREF] firmVersionMinor:\t #" + String(firmVersionMinor) + "#");
    Serial.println("-->[PREF] firmRevision:\t #" + String(firmRevision) + "#");
    Serial.println("-->[PREF] firmBranch:\t #" + firmBranch + "#");
    Serial.println("-->[PREF] firmFlavour:\t #" + firmFlavour + "#");
    Serial.println("-->[PREF] customCalValue: #" + String(customCalibrationValue) + "#");
    Serial.println("-->[PREF] tempOffset:\t #" + String(tempOffset, 1) + "#");
    Serial.println("-->[PREF] altitudeMeters:\t #" + String(altitudeMeters) + "#");
    Serial.println("-->[PREF] autoSelfCalibration:\t #" + String(autoSelfCalibration ? "Enabled" : "Disabled") + "#");
    Serial.println("-->[PREF] co2OrangeRange:\t #" + String(co2OrangeRange) + "#");
    Serial.println("-->[PREF] co2RedRange:\t #" + String(co2RedRange) + "#");
    Serial.println("-->[PREF] DisplayBrightness:\t #" + String(DisplayBrightness) + "#");
    Serial.println("-->[PREF] neopixBright:\t #" + String(neopixelBrightness) + "#");
    Serial.println("-->[PREF] selNeopxType:\t #" + String(selectedNeopixelType) + "#");
    Serial.println("-->[PREF] activeBLE is:\t#" + String(activeBLE ? "Enabled" : "Disabled") + "# (" + String(activeBLE) + ")");
    Serial.println("-->[PREF] activeWIFI is:\t#" + String(activeWIFI ? "Enabled" : "Disabled") + "# (" + String(activeWIFI) + ")");
    Serial.println("-->[PREF] activeMQTT is:\t#" + String(activeMQTT ? "Enabled" : "Disabled") + "# (" + String(activeMQTT) + ")");
    Serial.println("-->[PREF] activeESPNOW is:\t#" + String(activeESPNOW ? "Enabled" : "Disabled") + "# (" + String(activeESPNOW) + ")");
    Serial.println("-->[PREF] activeOTA is:\t#" + String(activeOTA ? "Enabled" : "Disabled") + "# (" + String(activeOTA) + ")");
    Serial.println("-->[PREF] rootTopic:\t#" + rootTopic + "#");
    Serial.println("-->[PREF] hasBattery:\t#" + String(hasBattery ? "Enabled" : "Disabled") + "# (" + String(hasBattery) + ")");
    Serial.println("-->[PREF] batDischgd:\t #" + String(batteryDischargedMillivolts) + "#");
    Serial.println("-->[PREF] batChargd:\t #" + String(batteryFullyChargedMillivolts) + "#");
    Serial.println("-->[PREF] vRef:\t #" + String(vRef) + "#");
    Serial.println("-->[PREF] mqttClientId:\t#" + mqttClientId + "#");
    Serial.println("-->[PREF] mqttShowInConsole:\t#" + String(mqttShowInConsole ? "Enabled" : "Disabled") + "# (" + String(mqttShowInConsole) + ")");
    Serial.println("-->[PREF] mqttBroker:\t#" + mqttBroker + "#");
    Serial.println("-->[PREF] mqttUser:\t#" + mqttUser + "#");
#ifndef WIFI_PRIVACY
    Serial.println("-->[PREF] mqttPass:\t#" + mqttPass + "#");
#endif
    Serial.println("-->[PREF] tToDispOff:\t #" + String(timeToDisplayOff) + "#");
    Serial.println("-->[PREF] tToPubMQTT:\t #" + String(timeBetweenMQTTPublish) + "#");
    Serial.println("-->[PREF] tToPubESPNow:\t #" + String(timeBetweenESPNowPublish) + "#");
    Serial.println("-->[PREF] tKeepAlMQTT:\t #" + String(timeToKeepAliveMQTT) + "#");
    Serial.println("-->[PREF] tKeepAlESPNow:\t #" + String(timeToKeepAliveESPNow) + "#");
    Serial.println("-->[PREF] dispOffOnExP:\t#" + String(displayOffOnExternalPower ? "Enabled" : "Disabled") + "# (" + String(displayOffOnExternalPower) + ")");
    Serial.println("-->[PREF] wifiSSID:\t#" + wifiSSID + "#");
#ifndef WIFI_PRIVACY
    Serial.println("-->[PREF] wifiPass:\t#" + wifiPass + "#");
#endif
    Serial.println("-->[PREF] hostName:\t#" + hostName + "#");

    // Fixed IP
    Serial.println("-->[PREF] useStaticIP:\t #" + String(useStaticIP) + "#");
    Serial.println("-->[PREF] staticIP:\t #" + staticIP.toString() + "#");
    Serial.println("-->[PREF] gateway:\t #" + gateway.toString() + "#");
    Serial.println("-->[PREF] subnet:\t #" + subnet.toString() + "#");
    Serial.println("-->[PREF] dns1:\t #" + dns1.toString() + "#");
    Serial.println("-->[PREF] dns2:\t #" + dns2.toString() + "#");

    Serial.println("-->[PREF] selCO2Sensor:\t #" + String(selectedCO2Sensor) + "#");
    Serial.println("-->[PREF] debugSensors is:\t#" + String(debugSensors ? "Enabled" : "Disabled") + "# (" + String(debugSensors) + ")");
    Serial.println("-->[PREF] displayReverse is:\t#" + String(displayReverse ? "Reversed" : "Normal") + "# (" + String(displayReverse) + ")");
    Serial.println("-->[PREF] showFahrenheit is:\t#" + String(showFahrenheit ? "Fahrenheit" : "Celsius") + "#");
    Serial.println("-->[PREF] measInterval:\t #" + String(measurementInterval) + "#");
    Serial.println("-->[PREF] sampInterval:\t #" + String(sampleInterval) + "#");
    Serial.println("-->[PREF] outModeRelay is:\t#" + String(outputsModeRelay ? "Relay" : "RGB LED") + "#");
    Serial.println("-->[PREF] channelESPNow:\t #" + String(channelESPNow) + "#");
    Serial.println("-->[PREF] boardIdESPNow:\t #" + String(boardIdESPNow) + "#");
    Serial.println("-->[PREF] peerESPNow:\t #" + String(peerESPNowAddress[0], HEX) + ":" + String(peerESPNowAddress[1], HEX) + ":" + String(peerESPNowAddress[2], HEX) + ":" + String(peerESPNowAddress[3], HEX) + ":" + String(peerESPNowAddress[4], HEX) + ":" + String(peerESPNowAddress[5], HEX) + "#");

    Serial.println("-->[PREF] showTemp:\t #" + String(displayShowTemperature ? "Show" : "Hide") + "#");
    Serial.println("-->[PREF] showHumidity:\t #" + String(displayShowHumidity ? "Show" : "Hide") + "#");
    Serial.println("-->[PREF] showBattery:\t #" + String(displayShowBattery ? "Show" : "Hide") + "#");
    Serial.println("-->[PREF] showBatteryVolt:\t #" + String(displayShowBatteryVoltage ? "Show" : "Hide") + "#");
    Serial.println("-->[PREF] showCO2:\t #" + String(displayShowCO2 ? "Show" : "Hide") + "#");
    Serial.println("-->[PREF] showPM25:\t #" + String(displayShowPM25 ? "Show" : "Hide") + "#");

    // Buzzer preferences
    Serial.println("-->[PREF] toneBuzzerBeep is:\t#" + String(toneBuzzerBeep) + "#");
    Serial.println("-->[PREF] durationBuzzerBeep is:\t#" + String(durationBuzzerBeep) + "#");
    Serial.println("-->[PREF] timeBetweenBuzzerBeeps is:\t#" + String(timeBetweenBuzzerBeeps) + "#");

    // Low power preferences
    Serial.printf("-->[PREF] lowPowerMode is:\t#%d#\n", deepSleepData.lowPowerMode);
    Serial.printf("-->[PREF] waitToDeep is:\t#%d#\n", deepSleepData.waitToGoDeepSleepOn1stBoot);
    Serial.printf("-->[PREF] timeSleeping is:\t#%d#\n", deepSleepData.timeSleeping);
    Serial.printf("-->[PREF] cyclsWifiConn is:\t#%d#\n", deepSleepData.cyclesLeftToWiFiConnect);
    Serial.printf("-->[PREF] cycRedrawDis is:\t#%d#\n", deepSleepData.cyclesLeftToRedrawDisplay);
    Serial.printf("-->[PREF] actBLEOnWake is:\t#%s#\n",
                  ((deepSleepData.activeBLEOnWake) ? "Enabled" : "Disabled"));
    Serial.printf("-->[PREF] actWifiOnWake is:\t#%s#\n",
                  ((deepSleepData.activeWifiOnWake) ? "Enabled" : "Disabled"));
    Serial.printf("-->[PREF] actMQTTOnWake is:\t#%s#\n",
                  ((deepSleepData.sendMQTTOnWake) ? "Enabled" : "Disabled"));
    Serial.printf("-->[PREF] actESPnowWake is:\t#%s#\n",
                  ((deepSleepData.sendESPNowOnWake) ? "Enabled" : "Disabled"));
    Serial.printf("-->[PREF] displayOnWake is:\t#%s#\n",
                  ((deepSleepData.displayOnWake) ? "Enabled" : "Disabled"));

    Serial.printf("-->[PREF] \n");
    // Captive Portal preferences
#ifdef SUPPORT_CAPTIVE_PORTAL
    Serial.println("-->[PREF] cpNoTimeout is:\t#" + String(captivePortalNoTimeout ? "Enabled" : "Disabled") + "#");
    Serial.println("-->[PREF] cpRelaxedSec is:\t#" + String(relaxedSecurity ? "Enabled" : "Disabled") + "#");
    Serial.println("-->[PREF] cpDebug is:\t#" + String(captivePortalDebug ? "Enabled" : "Disabled") + "#");
    Serial.println("-->[PREF] cpWaitTime is:\t#" + String(timeToWaitForCaptivePortal) + "#");
#endif

    Serial.println();
    delay(50);
}

void initPreferences() {
    // preferences.begin("CO2-Gadget", false);
    // if (preferences.clear()) {
    //     Serial.println("-->[PREF] Preferences cleared");
    // } else {
    //     Serial.println("-->[PREF] Preferences NOT cleared");
    // }
    // preferences.end();
    // delay(000);

    firmVersionMajor = getCO2GadgetMajorVersion();
    firmVersionMinor = getCO2GadgetMinorVersion();
    firmRevision = getCO2GadgetRevisionNumber();
    firmBranch = getCO2GadgetRevisionBranch();
    firmFlavour = FLAVOUR;

    preferences.begin("CO2-Gadget", false);
    prefVersion = preferences.getUInt("prefVersion", 0);
    prefRevision = preferences.getUInt("prefRevision", 0);
    firmVersionMajor = preferences.getUInt("firmVerMajor", 0);
    firmVersionMinor = preferences.getUInt("firmVerMinor", 0);
    firmRevision = preferences.getUInt("firmRevision", 0);
    firmBranch = preferences.getString("firmBranch", "");
    firmFlavour = preferences.getString("firmFlavour", "");
    customCalibrationValue = preferences.getUInt("customCalValue", 415);
    tempOffset = float(preferences.getFloat("tempOffset", 0));
    altitudeMeters = preferences.getUInt("altitudeMeters", 0);
    autoSelfCalibration = preferences.getBool("autoSelfCal", false);
    co2OrangeRange = preferences.getUInt("co2OrangeRange", 700);
    co2RedRange = preferences.getUInt("co2RedRange", 1000);
#ifdef LILYGO_T_DISPLAY_S3
    DisplayBrightness = preferences.getUInt("DisplayBright", 8);
#else
    DisplayBrightness = preferences.getUInt("DisplayBright", 100);
#endif
    neopixelBrightness = preferences.getUInt("neopixBright", 50);
    selectedNeopixelType = preferences.getUInt("selNeopxType", NEO_GRB + NEO_KHZ800);
    activeBLE = preferences.getBool("activeBLE", true);
    activeWIFI = preferences.getBool("activeWIFI", true);
    activeMQTT = preferences.getBool("activeMQTT", false);
    activeESPNOW = preferences.getBool("activeESPNOW", false);
    activeOTA = preferences.getBool("activeOTA", activeOTA);
    rootTopic = preferences.getString("rootTopic", rootTopic);
    mqttClientId = preferences.getString("mqttClientId", mqttClientId);
    mqttShowInConsole = preferences.getBool("mqttShowInCon", false);
    mqttBroker = preferences.getString("mqttBroker", mqttBroker).c_str();
    mqttUser = preferences.getString("mqttUser", mqttUser).c_str();
    mqttPass = preferences.getString("mqttPass", mqttPass).c_str();
    if (!activeWIFI) {  // If not WiFi active disable MQTT and save
        activeMQTT = false;
        preferences.putBool("activeMQTT", activeMQTT);
    }
    hasBattery = preferences.getBool("hasBattery", false);
    batteryDischargedMillivolts = preferences.getUInt("batDischgd", 3200);
    batteryFullyChargedMillivolts = preferences.getUInt("batChargd", 4200);
    vRef = preferences.getUInt("vRef", 930);  // Looks like, due to a bug, 930 is a goos starting number for vRef
    timeToDisplayOff = preferences.getUInt("tToDispOff", 60);
    timeBetweenMQTTPublish = preferences.getUInt("tToPubMQTT", 60);
    timeBetweenESPNowPublish = preferences.getUInt("tToPubESPNow", 60);
    timeToKeepAliveMQTT = preferences.getUInt("tKeepAlMQTT", 300);
    timeToKeepAliveESPNow = preferences.getUInt("tKeepAlESPNow", 300);

    displayOffOnExternalPower = preferences.getBool("dispOffOnExP", false);
    wifiSSID = preferences.getString("wifiSSID", wifiSSID).c_str();
    wifiPass = preferences.getString("wifiPass", wifiPass).c_str();
    hostName = preferences.getString("hostName", hostName).c_str();

    // Fixed IP
    useStaticIP = preferences.getBool("useStaticIP", false);
    staticIP.fromString(preferences.getString("staticIP", staticIP.toString()).c_str());
    gateway.fromString(preferences.getString("gateway", gateway.toString()).c_str());
    subnet.fromString(preferences.getString("subnet", subnet.toString()).c_str());
    dns1.fromString(preferences.getString("dns1", dns1.toString()).c_str());
    dns2.fromString(preferences.getString("dns2", dns2.toString()).c_str());

    selectedCO2Sensor = preferences.getUInt("selCO2Sensor", 0);
    debugSensors = preferences.getBool("debugSensors", false);
    displayReverse = preferences.getBool("displayReverse", false);
    showFahrenheit = preferences.getBool("showFahrenheit", false);
    measurementInterval = preferences.getUInt("measInterval", 10);
    sampleInterval = preferences.getUInt("sampInterval", 60);
    if (sampleInterval < 2) sampleInterval = 60;  // Default sample interval is 60 seconds
    outputsModeRelay = preferences.getBool("outModeRelay", false);
    channelESPNow = preferences.getUInt("channelESPNow", ESPNOW_WIFI_CH);
    boardIdESPNow = preferences.getUInt("boardIdESPNow", 0);

    size_t key_size = preferences.getBytesLength("peerESPNow");
    uint8_t buffer[key_size];
    preferences.getBytes("peerESPNow", buffer, key_size);

    if (key_size = sizeof(peerESPNowAddress)) {
        memcpy(peerESPNowAddress, buffer, sizeof(peerESPNowAddress));
    }

    displayShowTemperature = preferences.getBool("showTemp", true);
    displayShowHumidity = preferences.getBool("showHumidity", true);
    displayShowBattery = preferences.getBool("showBattery", true);
    displayShowBatteryVoltage = preferences.getBool("showBattVolt", false);
    displayShowCO2 = preferences.getBool("showCO2", true);
    displayShowPM25 = preferences.getBool("showPM25", true);

    // Retrieve buzzer preferences
    toneBuzzerBeep = preferences.getUInt("toneBzrBeep", BUZZER_TONE_MED);          // Frequency of the buzzer beep
    durationBuzzerBeep = preferences.getUInt("durBzrBeep", DURATION_BEEP_MEDIUM);  // Duration of the buzzer beep
    timeBetweenBuzzerBeeps = preferences.getUInt("timeBtwnBzr", 65535);            // Time between consecutive beeps

    // Low power preferences
    deepSleepData.lowPowerMode = preferences.getUInt("lowPowerMode", 0);
    deepSleepData.waitToGoDeepSleepOn1stBoot = preferences.getUInt("waitToDeep", 60);
    deepSleepData.timeSleeping = preferences.getUInt("timeSleeping", 60);
    deepSleepData.cyclesLeftToWiFiConnect = preferences.getUInt("cyclsWifiConn", 5);
    deepSleepData.activateWiFiEvery = deepSleepData.cyclesLeftToWiFiConnect;
    // deepSleepData.cyclesLeftToRedrawDisplay = preferences.getUInt("cycRedrawDis", 5);
    deepSleepData.redrawDisplayEveryCycles = deepSleepData.cyclesLeftToRedrawDisplay;
    deepSleepData.activeBLEOnWake = preferences.getBool("actBLEOnWake", true);
    deepSleepData.activeWifiOnWake = preferences.getBool("actWifiOnWake", false);
    deepSleepData.sendMQTTOnWake = preferences.getBool("actMQTTOnWake", false);
    deepSleepData.sendESPNowOnWake = preferences.getBool("actESPnowWake", false);
    deepSleepData.displayOnWake = preferences.getBool("displayOnWake", false);

    // Check if the values are within the expected range
    if ((deepSleepData.waitToGoDeepSleepOn1stBoot < 15) || (deepSleepData.waitToGoDeepSleepOn1stBoot > 900)) {
        deepSleepData.waitToGoDeepSleepOn1stBoot = 180;
        preferences.putUInt("waitToDeep", deepSleepData.waitToGoDeepSleepOn1stBoot);
    }

    preferences.end();
// Retrieve Captive Portal preferences
#ifdef SUPPORT_CAPTIVE_PORTAL
    captivePortalNoTimeout = preferences.getBool("cpNoTimeout", false);
    relaxedSecurity = preferences.getBool("cpRelaxedSec", false);
    captivePortalDebug = preferences.getBool("cpDebug", false);
    timeToWaitForCaptivePortal = preferences.getUInt("cpWaitTime", 60);
#endif

    rootTopic.trim();
    mqttClientId.trim();
    mqttBroker.trim();
    mqttUser.trim();
    mqttPass.trim();
    wifiSSID.trim();
    wifiPass.trim();
    hostName.trim();
    preferences.end();
#ifdef DEBUG_PREFERENCES
    printActualSettings();
#endif
    upgradePreferences();
}

void saveWifiCredentials() {
    Serial.println("-->[PREF] Saving WiFi credentials to NVR");
    preferences.begin("CO2-Gadget", false);
    preferences.putString("wifiSSID", wifiSSID);
    preferences.putString("wifiPass", wifiPass);
    preferences.end();
}

void putPreferences() {
    Serial.println("-->[PREF] Saving preferences to NVR");
    rootTopic.trim();
    mqttClientId.trim();
    mqttBroker.trim();
    mqttUser.trim();
    mqttPass.trim();
    wifiSSID.trim();
    wifiPass.trim();
    hostName.trim();
    // preferences.end();
    preferences.begin("CO2-Gadget", false);
    preferences.putUInt("prefVersion", prefVersion);
    preferences.putUInt("prefRevision", prefRevision);
    preferences.putUInt("firmVerMajor", firmVersionMajor);
    preferences.putUInt("firmVerMinor", firmVersionMinor);
    preferences.putUInt("firmRevision", firmRevision);
    preferences.putString("firmBranch", firmBranch);
    preferences.putString("firmFlavour", firmFlavour);
    preferences.putUInt("customCalValue", customCalibrationValue);
    preferences.putFloat("tempOffset", tempOffset);
    preferences.putUInt("altitudeMeters", altitudeMeters);
    preferences.putBool("autoSelfCal", autoSelfCalibration);
    preferences.putUInt("co2OrangeRange", co2OrangeRange);
    preferences.putUInt("co2RedRange", co2RedRange);
    preferences.putUInt("DisplayBright", DisplayBrightness);
    preferences.putUInt("neopixBright", neopixelBrightness);
    preferences.putUInt("selNeopxType", selectedNeopixelType);
    preferences.putBool("activeBLE", activeBLE);
    preferences.putBool("activeWIFI", activeWIFI);
    preferences.putBool("activeMQTT", activeMQTT);
    preferences.putBool("activeESPNOW", activeESPNOW);
    preferences.putBool("activeOTA", activeOTA);
    preferences.putString("rootTopic", rootTopic);
    preferences.putBool("hasBattery", hasBattery);
    preferences.putUInt("batDischgd", batteryDischargedMillivolts);
    preferences.putUInt("batChargd", batteryFullyChargedMillivolts);
    preferences.putUInt("vRef", vRef);
    preferences.putString("mqttClientId", mqttClientId);
    preferences.putBool("mqttShowInCon", mqttShowInConsole);
    preferences.putString("mqttBroker", mqttBroker);
    preferences.putString("mqttUser", mqttUser);
    preferences.putString("mqttPass", mqttPass);
    preferences.putUInt("tToDispOff", timeToDisplayOff);
    preferences.putUInt("tKeepAlMQTT", timeToKeepAliveMQTT);
    preferences.putUInt("tKeepAlESPNow", timeToKeepAliveESPNow);
    preferences.putUInt("tToPubMQTT", timeBetweenMQTTPublish);
    preferences.putUInt("tToPubESPNow", timeBetweenESPNowPublish);
    preferences.putBool("dispOffOnExP", displayOffOnExternalPower);
    preferences.putString("wifiSSID", wifiSSID);
    preferences.putString("wifiPass", wifiPass);
    preferences.putString("hostName", hostName);

    // Fixed IP
    preferences.putBool("useStaticIP", useStaticIP);
    preferences.putString("staticIP", staticIP.toString());
    preferences.putString("gateway", gateway.toString());
    preferences.putString("subnet", subnet.toString());
    preferences.putString("dns1", dns1.toString());
    preferences.putString("dns2", dns2.toString());

    preferences.putUInt("selCO2Sensor", selectedCO2Sensor);
    preferences.putBool("debugSensors", debugSensors);
    preferences.putBool("displayReverse", displayReverse);
    preferences.putBool("showFahrenheit", showFahrenheit);
    preferences.putUInt("measInterval", measurementInterval);
    preferences.putUInt("sampInterval", sampleInterval);
    preferences.putBool("outModeRelay", outputsModeRelay);
    preferences.putUInt("channelESPNow", channelESPNow);
    preferences.putUInt("boardIdESPNow", boardIdESPNow);
    preferences.putBytes("peerESPNow", peerESPNowAddress, 6);

    preferences.putBool("showTemp", displayShowTemperature);
    preferences.putBool("showHumidity", displayShowHumidity);
    preferences.putBool("showBattery", displayShowBattery);
    preferences.putBool("showBattVolt", displayShowBatteryVoltage);
    preferences.putBool("showCO2", displayShowCO2);
    preferences.putBool("showPM25", displayShowPM25);

    // Buzzer preferences
    preferences.putUInt("toneBzrBeep", toneBuzzerBeep);          // Buzzer frequency
    preferences.putUInt("durBzrBeep", durationBuzzerBeep);       // Buzzer duration
    preferences.putUInt("timeBtwnBzr", timeBetweenBuzzerBeeps);  // Time between beeps

    // Low power preferences
    preferences.putUInt("lowPowerMode", deepSleepData.lowPowerMode);
    preferences.putUInt("waitToDeep", deepSleepData.waitToGoDeepSleepOn1stBoot);
    preferences.putUInt("timeSleeping", deepSleepData.timeSleeping);
    preferences.putUInt("cyclsWifiConn", deepSleepData.cyclesLeftToWiFiConnect);
    preferences.putUInt("cycRedrawDis", deepSleepData.cyclesLeftToRedrawDisplay);
    preferences.putBool("actBLEOnWake", deepSleepData.activeBLEOnWake);
    preferences.putBool("actWifiOnWake", deepSleepData.activeWifiOnWake);
    preferences.putBool("actMQTTOnWake", deepSleepData.sendMQTTOnWake);
    preferences.putBool("actESPnowWake", deepSleepData.sendESPNowOnWake);
    preferences.putBool("displayOnWake", deepSleepData.displayOnWake);

    // Captive Portal preferences
#ifdef SUPPORT_CAPTIVE_PORTAL
    preferences.putBool("cpNoTimeout", captivePortalNoTimeout);
    // Do not save relaxedSecurity as true to avoid security issues
    // preferences.putBool("cpRelaxedSec", relaxedSecurity);
    preferences.putBool("cpRelaxedSec", false);
    preferences.putBool("cpDebug", captivePortalDebug);
    preferences.putUInt("cpWaitTime", timeToWaitForCaptivePortal);
#endif

    preferences.end();

#ifdef DEBUG_PREFERENCES
    printActualSettings();
#endif
}

String getCO2GadgetVersionAsJson() {
    String versionJson;
    JsonDocument doc;
    doc["firmVerMajor"] = getCO2GadgetMajorVersion();
    doc["firmVerMinor"] = getCO2GadgetMinorVersion();
    doc["firmRevision"] = getCO2GadgetRevisionNumber();
    doc["firmBranch"] = getCO2GadgetRevisionBranch();
    doc["firmFlavour"] = FLAVOUR;
    serializeJson(doc, versionJson);
#ifdef DEBUG_PREFERENCES
    Serial.println("-->[PREF] CO2 Gadget Version JSON: " + versionJson);
#endif
    return versionJson;
}

String getActualSettingsAsJson(bool includePasswords = false) {
    JsonDocument doc;

    doc["prefVersion"] = prefVersion;
    doc["prefRevision"] = prefRevision;
    doc["firmVerMajor"] = firmVersionMajor;
    doc["firmVerMinor"] = firmVersionMinor;
    doc["firmRevision"] = firmRevision;
    doc["firmBranch"] = firmBranch;
    doc["firmFlavour"] = firmFlavour;
    doc["customCalValue"] = customCalibrationValue;
    doc["tempOffset"] = String(tempOffset, 1);
    doc["altitudeMeters"] = altitudeMeters;
    doc["autoSelfCal"] = autoSelfCalibration;
    doc["co2OrangeRange"] = co2OrangeRange;
    doc["co2RedRange"] = co2RedRange;
    doc["DisplayBright"] = DisplayBrightness;
    doc["neopixBright"] = neopixelBrightness;
    doc["selNeopxType"] = selectedNeopixelType;
    doc["activeBLE"] = activeBLE;
    doc["activeWIFI"] = activeWIFI;
    doc["activeMQTT"] = activeMQTT;
    doc["activeESPNOW"] = activeESPNOW;
    doc["activeOTA"] = activeOTA;
    doc["rootTopic"] = rootTopic;
    doc["hasBattery"] = hasBattery;
    doc["batDischgd"] = batteryDischargedMillivolts;
    doc["batChargd"] = batteryFullyChargedMillivolts;
    doc["vRef"] = vRef;
    doc["mqttClientId"] = mqttClientId;
    doc["mqttShowInCon"] = mqttShowInConsole;
    doc["mqttBroker"] = mqttBroker;
    doc["mqttUser"] = mqttUser;
    // if includePasswords is false, do not include the password
    if (includePasswords) {
        doc["mqttPass"] = mqttPass;
    }
    doc["tToDispOff"] = timeToDisplayOff;
    doc["tKeepAlMQTT"] = timeToKeepAliveMQTT;
    doc["tKeepAlESPNow"] = timeToKeepAliveESPNow;
    doc["tToPubMQTT"] = timeBetweenMQTTPublish;
    doc["tToPubESPNow"] = timeBetweenESPNowPublish;
    doc["dispOffOnExP"] = displayOffOnExternalPower;
    doc["wifiSSID"] = wifiSSID;
    // if includePasswords is false, do not include the password
    if (includePasswords) {
        doc["wifiPass"] = wifiPass;
    }
    doc["hostName"] = hostName;

    // Fixed IP
    doc["useStaticIP"] = useStaticIP;
    doc["staticIP"] = staticIP.toString();
    doc["gateway"] = gateway.toString();
    doc["subnet"] = subnet.toString();
    doc["dns1"] = dns1.toString();
    doc["dns2"] = dns2.toString();

    doc["selCO2Sensor"] = selectedCO2Sensor;
    doc["debugSensors"] = debugSensors;
    doc["displayReverse"] = displayReverse;
    doc["showFahrenheit"] = showFahrenheit;
    doc["measurementInterval"] = measurementInterval;
    doc["outModeRelay"] = outputsModeRelay;
    doc["channelESPNow"] = channelESPNow;
    doc["boardIdESPNow"] = boardIdESPNow;

    // Convert peerESPNowAddress byte array to a string representation
    char peerAddressString[18];
    snprintf(peerAddressString, sizeof(peerAddressString),
             "%02X:%02X:%02X:%02X:%02X:%02X",
             peerESPNowAddress[0], peerESPNowAddress[1], peerESPNowAddress[2],
             peerESPNowAddress[3], peerESPNowAddress[4], peerESPNowAddress[5]);
    // Add the string representation to the JSON document
    doc["peerESPNowAddress"] = peerAddressString;

    doc["showTemp"] = displayShowTemperature;
    doc["showHumidity"] = displayShowHumidity;
    doc["showBattery"] = displayShowBattery;
    doc["showBattVolt"] = displayShowBatteryVoltage;
    doc["showCO2"] = displayShowCO2;
    doc["showPM25"] = displayShowPM25;
    doc["measInterval"] = measurementInterval;
    doc["sampInterval"] = sampleInterval;

    // Buzzer preferences
    doc["toneBzrBeep"] = toneBuzzerBeep;          // Buzzer frequency
    doc["durBzrBeep"] = durationBuzzerBeep;       // Buzzer duration
    doc["timeBtwnBzr"] = timeBetweenBuzzerBeeps;  // Time between beeps

    // Low power preferences
    doc["lowPowerMode"] = deepSleepData.lowPowerMode;
    doc["waitToDeep"] = deepSleepData.waitToGoDeepSleepOn1stBoot;
    doc["timeSleeping"] = deepSleepData.timeSleeping;
    doc["cyclsWifiConn"] = deepSleepData.cyclesLeftToWiFiConnect;
    doc["cycRedrawDis"] = deepSleepData.cyclesLeftToRedrawDisplay;
    doc["actBLEOnWake"] = deepSleepData.activeBLEOnWake;
    doc["actWifiOnWake"] = deepSleepData.activeWifiOnWake;
    doc["actMQTTOnWake"] = deepSleepData.sendMQTTOnWake;
    doc["actESPnowWake"] = deepSleepData.sendESPNowOnWake;
    doc["displayOnWake"] = deepSleepData.displayOnWake;

    // Captive Portal preferences
#ifdef SUPPORT_CAPTIVE_PORTAL
    doc["cpNoTimeout"] = captivePortalNoTimeout;
    if (relaxedSecurity) {
        doc["cpRelaxedSec"] = relaxedSecurity;
    }
    doc["cpDebug"] = captivePortalDebug;
    doc["cpWaitTime"] = timeToWaitForCaptivePortal;
#endif

    if (relaxedSecurity) {
        doc["relaxedSecurity"] = true;
    }

    String preferencesJson;
    serializeJson(doc, preferencesJson);
#ifdef DEBUG_PREFERENCES
    Serial.print("-->[PREF] Preferences JSON: ");
    Serial.println(preferencesJson);
#endif
    return preferencesJson;
}

bool handleSavePreferencesFromJSON(String jsonPreferences) {
    // Create a JSON object to store preferences
    DynamicJsonDocument JsonDocument(1024); // Asegúrate de ajustar el tamaño según sea necesario

    // Try to deserialize the JSON body from the request
    DeserializationError error = deserializeJson(JsonDocument, jsonPreferences);
    if (error) {
        // Handle the error when deserializing JSON
        Serial.print("Error deserializing JSON: ");
        Serial.println(error.c_str());
        return false;
    }

#ifdef DEBUG_PREFERENCES
    String debugMessage = "-->[PREF] JSON received (" + String(__func__) + "): " + jsonPreferences;
    Serial.println(debugMessage);
#endif

    // Save preferences to non-volatile memory (Preferences)
    try {
        preferences.begin("CO2-Gadget", false);

        if (JsonDocument.containsKey("customCalValue")) {
            customCalibrationValue = JsonDocument["customCalValue"];
        }
        if (JsonDocument.containsKey("tempOffset") && (tempOffset != float(JsonDocument["tempOffset"]))) {
            Serial.println("-->[PREF] Temp Offset changed from " + String(tempOffset) + " to " + String(JsonDocument["tempOffset"].as<String>()));
            tempOffset = float(JsonDocument["tempOffset"]);
            sensors.setTempOffset(tempOffset);
        }
        if (JsonDocument.containsKey("altitudeMeters")) {
            altitudeMeters = JsonDocument["altitudeMeters"];
        }
        if (JsonDocument.containsKey("autoSelfCal")) {
            autoSelfCalibration = JsonDocument["autoSelfCal"];
        }
        if (JsonDocument.containsKey("co2OrangeRange")) {
            co2OrangeRange = JsonDocument["co2OrangeRange"];
        }
        if (JsonDocument.containsKey("co2RedRange")) {
            co2RedRange = JsonDocument["co2RedRange"];
        }
        if (JsonDocument.containsKey("DisplayBright")) {
            if (DisplayBrightness != JsonDocument["DisplayBright"]) {
#ifdef LILYGO_T_DISPLAY_S3
                if (JsonDocument["DisplayBright"] > 16) {
                    JsonDocument["DisplayBright"] = 16;
                }
                if (JsonDocument["DisplayBright"] < 1) {
                    JsonDocument["DisplayBright"] = 1;
                }
#else
                if (JsonDocument["DisplayBright"] > 255) {
                    JsonDocument["DisplayBright"] = 255;
                }
                if (JsonDocument["DisplayBright"] < 1) {
                    JsonDocument["DisplayBright"] = 1;
                }
#endif
                DisplayBrightness = JsonDocument["DisplayBright"];
#if defined(SUPPORT_OLED) || defined(SUPPORT_TFT)
                // setDisplayBrightness(DisplayBrightness);
#endif
            }
        }
        if (JsonDocument.containsKey("neopixBright")) {
            neopixelBrightness = JsonDocument["neopixBright"];
        }
        if (JsonDocument.containsKey("selNeopxType")) {
            selectedNeopixelType = JsonDocument["selNeopxType"];
        }
        if (JsonDocument.containsKey("activeBLE")) {
            activeBLE = JsonDocument["activeBLE"];
        }
        if (JsonDocument.containsKey("activeWIFI")) {
            activeWIFI = JsonDocument["activeWIFI"];
        }
        if (JsonDocument.containsKey("activeMQTT")) {
            activeMQTT = JsonDocument["activeMQTT"];
        }
        if (JsonDocument.containsKey("activeESPNOW")) {
            activeESPNOW = JsonDocument["activeESPNOW"];
        }
        if (JsonDocument.containsKey("activeOTA")) {
            activeOTA = JsonDocument["activeOTA"];
        }
        if (JsonDocument.containsKey("rootTopic")) {
            rootTopic = JsonDocument["rootTopic"].as<String>().c_str();
        }
        if (JsonDocument.containsKey("hasBattery")) {
            hasBattery = JsonDocument["hasBattery"];
        }
        if (JsonDocument.containsKey("batDischgd")) {
            batteryDischargedMillivolts = JsonDocument["batDischgd"];
        }
        if (JsonDocument.containsKey("batChargd")) {
            batteryFullyChargedMillivolts = JsonDocument["batChargd"];
        }
        if (JsonDocument.containsKey("vRef") && (vRef != JsonDocument["vRef"])) {
            Serial.println("-->[PREF] vRef changed from " + String(vRef) + " to " + String(JsonDocument["vRef"].as<String>()));
            vRef = JsonDocument["vRef"];
            battery.begin(vRef, voltageDividerRatio, &asigmoidal);
            readBatteryVoltage();
        }
        if (JsonDocument.containsKey("mqttShowInCon")) {
            mqttShowInConsole = JsonDocument["mqttShowInCon"];
        }
        if (JsonDocument.containsKey("mqttClientId")) {
            mqttClientId = JsonDocument["mqttClientId"].as<String>().c_str();
        }
        if (JsonDocument.containsKey("mqttBroker")) {
            mqttBroker = JsonDocument["mqttBroker"].as<String>().c_str();
        }
        if (JsonDocument.containsKey("mqttUser")) {
            mqttUser = JsonDocument["mqttUser"].as<String>().c_str();
        }
        if (JsonDocument.containsKey("tToDispOff")) {
            timeToDisplayOff = JsonDocument["tToDispOff"];
        }
        if (JsonDocument.containsKey("tKeepAlMQTT")) {
            timeToKeepAliveMQTT = JsonDocument["tKeepAlMQTT"];
        }
        if (JsonDocument.containsKey("tKeepAlESPNow")) {
            timeToKeepAliveESPNow = JsonDocument["tKeepAlESPNow"];
        }
        if (JsonDocument.containsKey("tToPubMQTT")) {
            timeBetweenMQTTPublish = JsonDocument["tToPubMQTT"];
        }
        if (JsonDocument.containsKey("tToPubESPNow")) {
            timeBetweenESPNowPublish = JsonDocument["tToPubESPNow"];
        }
        if (JsonDocument.containsKey("dispOffOnExP")) {
            displayOffOnExternalPower = JsonDocument["dispOffOnExP"];
        }
        if (JsonDocument.containsKey("wifiSSID")) {
            wifiSSID = JsonDocument["wifiSSID"].as<String>().c_str();
        }
        if (JsonDocument.containsKey("hostName")) {
            hostName = JsonDocument["hostName"].as<String>().c_str();
        }

        // Fixed IP
        if (JsonDocument.containsKey("useStaticIP")) {
            useStaticIP = JsonDocument["useStaticIP"];
        }
        if (JsonDocument.containsKey("staticIP")) {
            staticIP.fromString(JsonDocument["staticIP"].as<String>());
        }
        if (JsonDocument.containsKey("gateway")) {
            gateway.fromString(JsonDocument["gateway"].as<String>());
        }
        if (JsonDocument.containsKey("subnet")) {
            subnet.fromString(JsonDocument["subnet"].as<String>());
        }
        if (JsonDocument.containsKey("dns1")) {
            dns1.fromString(JsonDocument["dns1"].as<String>());
        }
        if (JsonDocument.containsKey("dns2")) {
            dns2.fromString(JsonDocument["dns2"].as<String>());
        }

        if (JsonDocument.containsKey("selCO2Sensor")) {
            selectedCO2Sensor = JsonDocument["selCO2Sensor"];
        }
        if (JsonDocument.containsKey("debugSensors")) {
            debugSensors = JsonDocument["debugSensors"];
        }

#if defined(SUPPORT_TFT) || defined(SUPPORT_OLED) || defined(SUPPORT_EINK)
        if (JsonDocument.containsKey("displayReverse") && (displayReverse != JsonDocument["displayReverse"])) {
            Serial.println("-->[PREF] Display Reverse changed from " + String(displayReverse) + " to " + String(JsonDocument["displayReverse"].as<String>()));
            displayReverse = JsonDocument["displayReverse"];
            setDisplayReverse(displayReverse);
            reverseButtons(displayReverse);
            if (inMenu) isMenuDirty = true;
        }
#else
        if (JsonDocument.containsKey("displayReverse")) {
            displayReverse = JsonDocument["displayReverse"];
            reverseButtons(displayReverse);
        }
#endif
        if (JsonDocument.containsKey("showFahrenheit")) {
            showFahrenheit = JsonDocument["showFahrenheit"];
        }
        if (JsonDocument.containsKey("measurementInterval")) {
            measurementInterval = JsonDocument["measurementInterval"];
        }
        if (JsonDocument.containsKey("sampleInterval")) {
            sampleInterval = JsonDocument["sampleInterval"];
        }
        if (JsonDocument.containsKey("outModeRelay")) {
            outputsModeRelay = JsonDocument["outModeRelay"];
        }
        if (JsonDocument.containsKey("channelESPNow")) {
            channelESPNow = JsonDocument["channelESPNow"];
        }
        if (JsonDocument.containsKey("boardIdESPNow")) {
            boardIdESPNow = JsonDocument["boardIdESPNow"];
        }

        // Get the MAC address for peerESPNowAddress as a string from JSON
        if (JsonDocument.containsKey("peerESPNowAddress")) {
            String peerESPNowAddressStr = JsonDocument["peerESPNowAddress"].as<String>();
            // Convert the string to an array of uint8_t
            uint8_t peerESPNowAddress[6];
            const char* peerESPNowAddressChar = peerESPNowAddressStr.c_str();
            for (int i = 0; i < 6; i++) {
                peerESPNowAddress[i] = strtoul(peerESPNowAddressChar + i * 3, NULL, 16);
            }
        }

        if (JsonDocument.containsKey("showTemp") && (displayShowTemperature != JsonDocument["showTemp"])) {
            Serial.println("-->[PREF] Display Temperature changed from " + String(displayShowTemperature) + " to " + String(JsonDocument["showTemp"].as<String>()));
            displayShowTemperature = JsonDocument["showTemp"];
            shouldRedrawDisplay = true;
        }

        if (JsonDocument.containsKey("showHumidity") && (displayShowHumidity != JsonDocument["showHumidity"])) {
            Serial.println("-->[PREF] Display Humidity changed from " + String(displayShowHumidity) + " to " + String(JsonDocument["showHumidity"].as<String>()));
            displayShowHumidity = JsonDocument["showHumidity"];
            shouldRedrawDisplay = true;
        }

        if (JsonDocument.containsKey("showBattery") && (displayShowBattery != JsonDocument["showBattery"])) {
            Serial.println("-->[PREF] Display Battery changed from " + String(displayShowBattery) + " to " + String(JsonDocument["showBattery"].as<String>()));
            displayShowBattery = JsonDocument["showBattery"];
            shouldRedrawDisplay = true;
        }

        if (JsonDocument.containsKey("showBattVolt") && (displayShowBatteryVoltage != JsonDocument["showBattVolt"])) {
            Serial.println("-->[PREF] Display Battery Voltage changed from " + String(displayShowBatteryVoltage) + " to " + String(JsonDocument["showBattVolt"].as<String>()));
            displayShowBatteryVoltage = JsonDocument["showBattVolt"];
            shouldRedrawDisplay = true;
        }

        if (JsonDocument.containsKey("showCO2") && (displayShowCO2 != JsonDocument["showCO2"])) {
            Serial.println("-->[PREF] Display CO2 changed from " + String(displayShowCO2) + " to " + String(JsonDocument["showCO2"].as<String>()));
            displayShowCO2 = JsonDocument["showCO2"];
            shouldRedrawDisplay = true;
        }

        if (JsonDocument.containsKey("showPM25") && (displayShowPM25 != JsonDocument["showPM25"])) {
            Serial.println("-->[PREF] Display PM2.5 changed from " + String(displayShowPM25) + " to " + String(JsonDocument["showPM25"].as<String>()));
            displayShowPM25 = JsonDocument["showPM25"];
            shouldRedrawDisplay = true;
        }

        // Buzzer preferences
        if (JsonDocument.containsKey("toneBzrBeep")) {
            toneBuzzerBeep = JsonDocument["toneBzrBeep"];
        }
        if (JsonDocument.containsKey("durBzrBeep")) {
            durationBuzzerBeep = JsonDocument["durBzrBeep"];
        }
        if (JsonDocument.containsKey("timeBtwnBzr")) {
            timeBetweenBuzzerBeeps = JsonDocument["timeBtwnBzr"];
        }

        // Low power preferences
        if (JsonDocument.containsKey("lowPowerMode")) {
            deepSleepData.lowPowerMode = JsonDocument["lowPowerMode"];
        }
        if (JsonDocument.containsKey("waitToDeep")) {
            deepSleepData.waitToGoDeepSleepOn1stBoot = JsonDocument["waitToDeep"];
        }
        if (JsonDocument.containsKey("timeSleeping")) {
            deepSleepData.timeSleeping = JsonDocument["timeSleeping"];
        }
        if (JsonDocument.containsKey("cyclsWifiConn")) {
            deepSleepData.cyclesLeftToWiFiConnect = JsonDocument["cyclsWifiConn"];
        }
        // if (JsonDocument.containsKey("cycRedrawDis")) {
        //     deepSleepData.cyclesLeftToRedrawDisplay = JsonDocument["cycRedrawDis"];
        // }
        if (JsonDocument.containsKey("actBLEOnWake")) {
            deepSleepData.activeBLEOnWake = JsonDocument["actBLEOnWake"];
        }
        if (JsonDocument.containsKey("actWifiOnWake")) {
            deepSleepData.activeWifiOnWake = JsonDocument["actWifiOnWake"];
        }
        if (JsonDocument.containsKey("actMQTTOnWake")) {
            deepSleepData.sendMQTTOnWake = JsonDocument["actMQTTOnWake"];
        }
        if (JsonDocument.containsKey("actESPnowWake")) {
            deepSleepData.sendESPNowOnWake = JsonDocument["actESPnowWake"];
        }
        if (JsonDocument.containsKey("displayOnWake")) {
            deepSleepData.displayOnWake = JsonDocument["displayOnWake"];
        }

        // Captive Portal preferences
#ifdef SUPPORT_CAPTIVE_PORTAL
        if (JsonDocument.containsKey("cpNoTimeout")) {
#ifdef DEBUG_CAPTIVE_PORTAL
            Serial.println("-->[CAPT] cpNoTimeout is present. Setting value to: " + String(JsonDocument["cpNoTimeout"].as<String>()));
#endif
            captivePortalNoTimeout = JsonDocument["cpNoTimeout"];
        }
        if (JsonDocument.containsKey("cpRelaxedSec")) {
#ifdef DEBUG_CAPTIVE_PORTAL
            Serial.println("-->[CAPT] cpRelaxedSec is present. Setting value to: " + String(JsonDocument["cpRelaxedSec"].as<String>()));
#endif
            relaxedSecurity = JsonDocument["cpRelaxedSec"];
        }
        if (JsonDocument.containsKey("cpDebug")) {
#ifdef DEBUG_CAPTIVE_PORTAL
            Serial.println("-->[CAPT] cpDebug is present. Setting value to: " + String(JsonDocument["cpDebug"].as<String>()));
#endif
            captivePortalDebug = JsonDocument["cpDebug"];
        }
        if (JsonDocument.containsKey("cpWaitTime")) {
#ifdef DEBUG_CAPTIVE_PORTAL
            Serial.println("-->[CAPT] cpWaitTime is present. Setting value to: " + String((long)JsonDocument["cpWaitTime"]));
#endif
            timeToWaitForCaptivePortal = JsonDocument["cpWaitTime"];
        }
#endif

        // If JsonDocument["wifiPass"] is present and different from the current one, update the wifiPass variable
        if (JsonDocument.containsKey("wifiPass") && (wifiPass != JsonDocument["wifiPass"].as<String>().c_str())) {
            Serial.println("-->[PREF] WiFi Password changed. Updating WiFi Password in preferences");
            wifiPass = JsonDocument["wifiPass"].as<String>().c_str();
            wifiChanged = true;
        }
        // If JsonDocument["mqttPass"] is present and different from the current one, update the mqttPass variable
        if (JsonDocument.containsKey("mqttPass") && (mqttPass != JsonDocument["mqttPass"].as<String>().c_str())) {
            Serial.println("-->[PREF] MQTT Password changed. Updating MQTT Password in preferences");
            mqttPass = JsonDocument["mqttPass"].as<String>().c_str();
            wifiChanged = true;
        }
        preferences.end();
    } catch (const std::exception& e) {
        // Manage error while storing preferences
        Serial.print("Error storing preferences: ");
        Serial.println(e.what());
        return false;
    }

    putPreferences();
    return true;
}

bool setPreferenceValue(String key, String value) {
    preferences.begin("CO2-Gadget", false);

    bool success = false;

    // Determine the type of the value and store it accordingly
    if (value.equalsIgnoreCase("true") || value.equalsIgnoreCase("false")) {
        bool boolValue = value.equalsIgnoreCase("true");
        success = preferences.putBool(key.c_str(), boolValue);
        Serial.println("-->[PREF] Setting boolean value for preference key: " + key + " with value: " + value + " (Success: " + String(success) + ")");
    } else if (value.toInt() != 0 || value == "0") {
        int intValue = value.toInt();
        success = preferences.putInt(key.c_str(), intValue);
        Serial.println("-->[PREF] Setting integer value for preference key: " + key + " with value: " + value + " (Success: " + String(success) + ")");
    } else if (value.toFloat() != 0.0 || value == "0.0") {
        float floatValue = value.toFloat();
        success = preferences.putFloat(key.c_str(), floatValue);
        Serial.println("-->[PREF] Setting float value for preference key: " + key + " with value: " + value + " (Success: " + String(success) + ")");
    } else {
        success = preferences.putString(key.c_str(), value);
        Serial.println("-->[PREF] Setting string value for preference key: " + key + " with value: " + value + " (Success: " + String(success) + ")");
    }

    preferences.end();
    return success;
}

#endif  // CO2_Gadget_Preferences_h