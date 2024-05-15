// https://github.com/Makerfabs/Makerfabs-ESP32-UWB

#include "main.h"

#include <WiFi.h>

#define CALIBRATION_TAG

void setup()
{
    Serial.begin(115200);
    delay(1000);
    // init the configuration
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    DW1000Ranging.initCommunication(PIN_RST, PIN_SS,
        PIN_IRQ); // Reset, CS, IRQ pin
    // define the sketch as anchor. It will be great to dynamically change the
    // type of module
    DW1000Ranging.attachNewRange(newRange);
    DW1000Ranging.attachNewDevice(newDevice);
    DW1000Ranging.attachInactiveDevice(inactiveDevice);
    // Enable the filter to smooth the distance
    // DW1000Ranging.useRangeFilter(true);

    // we start the module as a tag
    // ude the mac address as the address
    char macBuffer[20];
    WiFi.macAddress().toCharArray(macBuffer, 18);
#ifndef CALIBRATION_TAG
    DW1000Ranging.startAsTag(macBuffer, DW1000.MODE_SHORTDATA_FAST_ACCURACY,
        false);
#else
    DW1000Ranging.startAsTag("FF:FF:FF:FF:FF:FF", DW1000.MODE_SHORTDATA_FAST_ACCURACY,
        false);
#endif
}

void loop()
{
    // put your main code here, to run repeatedly:
    DW1000Ranging.loop();
}

void newRange()
{
    Serial.print("from: ");
    Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
    Serial.print("\t Range: ");
    Serial.print(DW1000Ranging.getDistantDevice()->getRange());
    Serial.print(" m");
    Serial.print("\t RX power: ");
    Serial.print(DW1000Ranging.getDistantDevice()->getRXPower());
    Serial.println(" dBm");
}

void newDevice(DW1000Device* device)
{
    Serial.print("ranging init; 1 device added ! -> ");
    Serial.print(" short:");
    Serial.println(device->getShortAddress(), HEX);
}

void inactiveDevice(DW1000Device* device)
{
    Serial.print("delete inactive device: ");
    Serial.println(device->getShortAddress(), HEX);
}