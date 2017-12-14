//
// Created by ryo on 2017/12/02.
//
#include <SPI.h>
#include "Arduino.h"
#include "StakSPI/SPISlave.h"

SPISlave SPISlave;

void setup() {
    Serial.begin(115200);
    SPISlave.begin();
}

void loop() {

    if (Serial.read() != -1) {
        SPISlave.send("マスターにデータを贈りたい");
    }

    SPISlave.handler();
}

void SPISlave::SLAVE_CALLBACK(uint8_t *message, type_t data_type, size_t length) {

    switch (data_type) {
        case UINT8:
            for (; uint8_t i = *message; message++) {
                Serial.print(i);
                Serial.print(',');
            }
            Serial.print('\n');
            break;
        case UINT16:
            uint8_t test;
            for (int pos = 1; uint8_t i = *message; message++, pos++) {
                if (pos % 2 == 0) {
                    Serial.print(makeWord(test, i));
//                    Serial.print(test);
//                    Serial.print(",");
//                    Serial.print(i);
                    Serial.print(",");
                } else {
                    test = i;
                }
            }

            Serial.print('\n');
            break;
        case CHAR:
            Serial.println((char *) message);
            break;
    }

}