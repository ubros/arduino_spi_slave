//
// Created by ryo on 2017/12/02.
//
#include <SPI.h>
#include "Arduino.h"
#include "StakSPI/SPISlave.h"

SPISlave SPISlave;

void setup() {
    Serial.begin(115200);
    delay(100);

    SPISlave.begin();
}

void loop() {

    if (Serial.read() != -1) {
        SPISlave.send("マスターにデータを贈りたい");
    }

    SPISlave.handler();
}

void SPISlave::SLAVE_CALLBACK(char *message) {
    Serial.println(message);
}