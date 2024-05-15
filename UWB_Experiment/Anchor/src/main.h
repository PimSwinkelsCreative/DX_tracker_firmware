#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <Wifi.h>

#include "DW1000Ranging.h"

#define CALIBRATION_DISTANCE 1.83
#define CALIBRATION_SHORT_ID 0xFFFF

#define ANCHOR_ID 2

#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23

#define PIN_SS 4
#define PIN_RST 27
#define PIN_IRQ 34

const uint8_t primes[] = {17,19,23,31,37,41,43,47,53,59,61,67,71};

void inactiveDevice(DW1000Device *device);

void newBlink(DW1000Device *device);

void newRange();

void StartAnchor();