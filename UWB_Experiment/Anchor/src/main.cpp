// https://github.com/Makerfabs/Makerfabs-ESP32-UWB

#include "main.h"

// calibration variables:
uint16_t this_anchor_Adelay = 16600; // starting value
uint16_t Adelay_delta = 100; // initial binary search step size

char macAddress[20];

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    delay(1000);

    // init the configuration
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    DW1000Ranging.initCommunication(PIN_RST, PIN_SS,
        PIN_IRQ); // Reset, CS, IRQ pin
    // define the sketch as anchor. It will be great to dynamically change the
    // type of module
    DW1000Ranging.attachNewRange(newRange);
    DW1000Ranging.attachBlinkDevice(newBlink);
    DW1000Ranging.attachInactiveDevice(inactiveDevice);

    DW1000.setAntennaDelay(this_anchor_Adelay);

    // Enable the filter to smooth the distance
    // DW1000Ranging.useRangeFilter(true);
    WiFi.macAddress().toCharArray(macAddress, 18);

    StartAnchor(); // restart the anchor to apply the updated antenna delay
}

void loop()
{
    // put your main code here, to run repeatedly:
    DW1000Ranging.loop();
}

void newRange()
{
    uint16_t tagShortAddress = DW1000Ranging.getDistantDevice()->getShortAddress();
    float distance = DW1000Ranging.getDistantDevice()->getRange();

    if (tagShortAddress == CALIBRATION_SHORT_ID) {
        static float last_delta = 0.0;
        float this_delta = distance - CALIBRATION_DISTANCE; // error in measured distance
        if (Adelay_delta < 1 || distance < 0 || distance > 4 * CALIBRATION_DISTANCE || distance < CALIBRATION_DISTANCE / 4)
            return; // stop calibrating when error is very small

        if (this_delta * last_delta < 0.0) {
            Adelay_delta = Adelay_delta / 2; // sign changed, reduce step size
        }
        last_delta = this_delta;

        if (this_delta > 0.0) {
            this_anchor_Adelay += Adelay_delta; // new trial Adelay
        } else {
            this_anchor_Adelay -= Adelay_delta;
        }
        // DW1000Ranging.initCommunication(PIN_RST, PIN_SS,
        //     PIN_IRQ); // Reset, CS, IRQ pin

        DW1000.setAntennaDelay(this_anchor_Adelay);
        Serial.println("measured Distance: " + String(distance) + "\ttarget distance: " + String(CALIBRATION_DISTANCE) + "\tdelta: " + String(this_delta) + "/tAntenna delay set to: " + String(this_anchor_Adelay));
        StartAnchor(); // restart the anchor to apply the updated antenna delay

    } else {
        Serial.print("ANCHORID:");
        Serial.print(ANCHOR_ID);
        Serial.print("\tTAGID:");
        Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
        Serial.print("\tDIST:");
        Serial.println(distance);
    }
}

void newBlink(DW1000Device* device)
{
    Serial.print("NEW:");
    Serial.println(device->getShortAddress(), HEX);
}

void inactiveDevice(DW1000Device* device)
{
    Serial.print("DEL:");
    Serial.println(device->getShortAddress(), HEX);
}

void StartAnchor()
{
    DW1000Ranging.startAsAnchor(macAddress, DW1000.MODE_LONGDATA_RANGE_LOWPOWER);
    // DW1000Ranging.startAsAnchor(macAddress,
    // DW1000.MODE_SHORTDATA_FAST_LOWPOWER);
    // DW1000Ranging.startAsAnchor(macAddress,
    // DW1000.MODE_LONGDATA_FAST_LOWPOWER);
    // DW1000Ranging.startAsAnchor(macAddress,DW1000.MODE_SHORTDATA_FAST_ACCURACY);
    // DW1000Ranging.startAsAnchor(macAddress,
    // DW1000.MODE_LONGDATA_FAST_ACCURACY);
    // DW1000Ranging.startAsAnchor(macAddress, DW1000.MODE_LONGDATA_RANGE_ACCURACY);
}