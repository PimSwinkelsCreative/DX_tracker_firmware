#pragma once

#include <Arduino.h>
#include <SPI.h>

#include "DW1000Ranging.h"

// #define ANCHOR_ADD "86:17:5B:D5:A9:9A:E2:9C"

#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23

#define PIN_SS 4
#define PIN_RST 27
#define PIN_IRQ 34

void inactiveDevice(DW1000Device *device);

void newDevice(DW1000Device *device);

void newRange();