//
// Created by ryo on 2017/12/02.
//
#include <SPI.h>
#include <IRremote.h>
#include "Arduino.h"
#include "StakSPI/SPISlave.h"

SPISlave SPISlave;
IRsend irsend;

int khz = 38;

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

void SPISlave::SLAVE_CALLBACK(universal_t *message, size_t length) {

    Serial.print("サイズ:");
    Serial.println(length);

    unsigned int signal[length];

    for (int i = 0; length > i; i++) {
        signal[i] = message[i].bit16;
    }

    irsend.sendRaw(signal, sizeof(signal) / sizeof(signal[0]), khz);

    Serial.print("\n\n");
}