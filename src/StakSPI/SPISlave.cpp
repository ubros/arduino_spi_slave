//
// Created by ryo on 2017/12/03.
//

#include "SPISlave.h"

/**************************************************************
 * public method
 **************************************************************/

void SPISlave::begin() {
    Serial.println("Slave init");
    pinMode(MISO, OUTPUT);
    SPI.setClockDivider(SPI_CLOCK_DIV128);
    SPCR |= _BV(SPE);
    SPI.attachInterrupt();
    status_t.status = STANDBY;

    data_t.bit_order = false;
    data_t.pos = 0;
    data_t.send_pos = 0;
    data_t.process_it = false;
}

void SPISlave::handler() {
    if (!data_t.process_it)
        return;

    switch (status_t.status) {
        case RECEIVE_READY:
            SPDR = SUCCESS;
            data_t.pos = 0;
            data_t.process_it = false;
            break;
        case RECEIVE_COMPLETE:
            SPDR = SUCCESS;
            status_t.status = STANDBY;
//            data_t.buf[data_t.pos] = 0;
            Serial.println(data_t.pos);
            SLAVE_CALLBACK(data_t.buf, (size_t) data_t.pos);
            data_t.bit_order = false;
            data_t.process_it = false;
            break;
        case SEND_READY:
            SPDR = SUCCESS;
            data_t.process_it = false;
            data_t.send_pos = 0;
            break;
        case SEND_TRANSFER:
            if (data_t.send_pos == data_t.pos) {
                SPDR = SEND_COMPLETE;
                data_t.process_it = false;
                return;
            }
//            SPDR = data_t.buf[data_t.send_pos++];
            data_t.process_it = false;
            break;
        case SEND_COMPLETE:
            SPDR = SUCCESS;
            data_t.pos = 0;
//            data_t.buf[data_t.pos] = 0;
            status_t.status = STANDBY;
            data_t.process_it = false;
            break;
        default:
            break;
    }
}

void SPISlave::send(const char *message) {
//    data_t.pos = 0;
//    data_t.buf[data_t.pos] = 0;
//    data_t.process_it = false;
//    for (; char c = *message; message++) {
//        data_t.buf[data_t.pos++] = c;
//    }
//    status_t.status = SEND_STANDBY;
}

/**************************************************************
 * private method
 **************************************************************/

/**************************************************************
 * callback method
 **************************************************************/

ISR(SPI_STC_vect) {
    byte c = SPDR;

    switch (c) {
        case RECEIVE_READY:
            status_t.status = RECEIVE_READY;
            data_t.process_it = true;
            return;
        case RECEIVE_COMPLETE:
            status_t.status = RECEIVE_COMPLETE;
            data_t.process_it = true;
            return;
        case SEND_READY:
            if (status_t.status == SEND_STANDBY) {
                status_t.status = SEND_READY;
                data_t.process_it = true;
            }
            break;
        case SEND_TRANSFER:
            status_t.status = SEND_TRANSFER;
            data_t.process_it = true;
            break;
        case SEND_COMPLETE:
            status_t.status = SEND_COMPLETE;
            data_t.process_it = true;
            break;
        case ZERO:
            if (status_t.status != RECEIVE_TRANSFER)
                return;
        default:
            break;
    }

    switch (status_t.status) {
        case RECEIVE_READY:
        case RECEIVE_TRANSFER:
            status_t.status = RECEIVE_TRANSFER;
            data_t.buf[data_t.pos].bit8[data_t.bit_order] = c;
            if (data_t.bit_order) {
                data_t.pos++;
            }
            data_t.bit_order = !data_t.bit_order;

            return;
        default:
            break;
    }
}