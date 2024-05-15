// https://github.com/Makerfabs/Makerfabs-ESP32-UWB

#include "main.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  // init the configuration
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  DW1000Ranging.initCommunication(PIN_RST, PIN_SS,
                                  PIN_IRQ);  // Reset, CS, IRQ pin
  // define the sketch as anchor. It will be great to dynamically change the
  // type of module
  DW1000Ranging.attachNewRange(newRange);
  DW1000Ranging.attachBlinkDevice(newBlink);
  DW1000Ranging.attachInactiveDevice(inactiveDevice);

  DW1000.setAntennaDelay(16600);
  // Enable the filter to smooth the distance
  // DW1000Ranging.useRangeFilter(true);

  // we start the module as an anchor
  //  DW1000Ranging.startAsAnchor("82:17:5B:D5:A9:9A:E2:9C",
  //  DW1000.MODE_LONGDATA_RANGE_ACCURACY);

  char macBuffer[20];
  WiFi.macAddress().toCharArray(macBuffer, 18);
  DW1000Ranging.startAsAnchor(macBuffer, DW1000.MODE_LONGDATA_RANGE_LOWPOWER);
  // DW1000Ranging.startAsAnchor(ANCHOR_ADD,
  // DW1000.MODE_SHORTDATA_FAST_LOWPOWER);
  // DW1000Ranging.startAsAnchor(ANCHOR_ADD,
  // DW1000.MODE_LONGDATA_FAST_LOWPOWER);
  // DW1000Ranging.startAsAnchor(ANCHOR_ADD,
  // DW1000.MODE_SHORTDATA_FAST_ACCURACY);
  // DW1000Ranging.startAsAnchor(ANCHOR_ADD,
  // DW1000.MODE_LONGDATA_FAST_ACCURACY);
  // DW1000Ranging.startAsAnchor(ANCHOR_ADD,
  // DW1000.MODE_LONGDATA_RANGE_ACCURACY);
}

void loop() {
  // put your main code here, to run repeatedly:
  DW1000Ranging.loop();
  delay(primes[ANCHOR_ID]);
}

void newRange() {
  float distance = DW1000Ranging.getDistantDevice()->getRange();
  // if (distance < 0.1 || distance > 50) return;

  Serial.print("ANCHORID:");
  Serial.print(ANCHOR_ID);
  Serial.print("\tTAGID:");
  Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
  Serial.print("\tDIST:");
  Serial.println(distance);
}

void newBlink(DW1000Device *device) {
  Serial.print("NEW:");
  Serial.println(device->getShortAddress(), HEX);
}

void inactiveDevice(DW1000Device *device) {
  Serial.print("DEL:");
  Serial.println(device->getShortAddress(), HEX);
}