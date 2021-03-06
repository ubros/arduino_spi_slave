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
    SPI.setClockDivider(SPI_CLOCK_DIV4);
    SPCR |= _BV(SPE);
    SPI.attachInterrupt();
    status_t.status = STANDBY;

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
            data_t.process_it = false;
            break;
        case RECEIVE_NEGOTIATION_UINT8:
            SPDR = SUCCESS;
            data_t.type = UINT8;
            data_t.process_it = false;
            break;
        case RECEIVE_NEGOTIATION_UINT16:
            SPDR = SUCCESS;
            data_t.type = UINT16;
            data_t.process_it = false;
            break;
        case RECEIVE_NEGOTIATION_CHAR:
            SPDR = SUCCESS;
            data_t.type = CHAR;
            data_t.process_it = false;
            break;
        case RECEIVE_COMPLETE:
            SPDR = SUCCESS;
            status_t.status = STANDBY;
            data_t.buf[data_t.pos] = 0;
            data_t.pos = 0;
            data_t.process_it = false;
            SLAVE_CALLBACK(data_t.buf, data_t.type, sizeof(data_t.buf));
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
            SPDR = data_t.buf[data_t.send_pos++];
            data_t.process_it = false;
            break;
        case SEND_COMPLETE:
            SPDR = SUCCESS;
            data_t.pos = 0;
            data_t.buf[data_t.pos] = 0;
            status_t.status = STANDBY;
            data_t.process_it = false;
            break;
        default:
            break;
    }
}

void SPISlave::send(const char *message) {
    data_t.pos = 0;
    data_t.buf[data_t.pos] = 0;
    data_t.process_it = false;
    for (; char c = *message; message++) {
        data_t.buf[data_t.pos++] = c;
    }
    status_t.status = SEND_STANDBY;
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
            if (status_t.status != STANDBY)
                break;
            status_t.status = RECEIVE_READY;
            data_t.process_it = true;
            return;
        case RECEIVE_NEGOTIATION_UINT8:
            if (status_t.status != RECEIVE_READY)
                break;
            status_t.status = RECEIVE_NEGOTIATION_UINT8;
            data_t.process_it = true;
            return;
        case RECEIVE_NEGOTIATION_UINT16:
            if (status_t.status != RECEIVE_READY)
                break;
            status_t.status = RECEIVE_NEGOTIATION_UINT16;
            data_t.process_it = true;
            return;
        case RECEIVE_NEGOTIATION_CHAR:
            if (status_t.status != RECEIVE_READY)
                break;
            status_t.status = RECEIVE_NEGOTIATION_CHAR;
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
            return;
        default:
            break;
    }

    switch (status_t.status) {
        case RECEIVE_NEGOTIATION_UINT8:
        case RECEIVE_NEGOTIATION_UINT16:
        case RECEIVE_NEGOTIATION_CHAR:
        case RECEIVE_TRANSFER:
            status_t.status = RECEIVE_TRANSFER;
            data_t.buf[data_t.pos++] = c;
            return;
        default:
            break;
    }
}