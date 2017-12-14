//
// Created by ryo on 2017/12/03.
//

#ifndef ARDUINO_SPI_SLAVE_SPISLAVE_H
#define ARDUINO_SPI_SLAVE_SPISLAVE_H

#include "Arduino.h"
#include "SPI.h"

typedef enum {
    STANDBY = '\1',             //送受信可能
    RECEIVE_READY = '\2',        //受信準備
    RECEIVE_NEGOTIATION_UINT8 = '\15',      //受信種別決定
    RECEIVE_NEGOTIATION_UINT16 = '\16',      //受信種別決定
    RECEIVE_NEGOTIATION_CHAR = '\17',      //受信種別決定
    RECEIVE_TRANSFER = '\6',    //受信中
    RECEIVE_COMPLETE = '\77',    //受信完了
    SEND_STANDBY = '\10',           //送信準備
    SEND_READY = '\11',           //送信準備
    SEND_TRANSFER = '\12',       //送信中
    SEND_COMPLETE = '\13',       //送信完了
} STATUS;

typedef enum {
    ZERO = 0,                   //ゼロデータ
    SUCCESS = '\20',             //成功
    ERROR = '\21',               //失敗
} RESPONSE;

struct {
    STATUS status;
} status_t;

typedef enum {
    UINT8 = 0x01,
    UINT16 = 0x02,
    CHAR = 0x03,
} type_t;

struct {
    type_t type;
    uint8_t buf[1024];
    volatile byte pos;
    volatile byte send_pos;
    volatile boolean process_it;
} data_t;


class SPISlave {

public:
    void begin();

    void handler();

    void send(const char *message);

    void SLAVE_CALLBACK(uint8_t *message, type_t type, size_t length);

private:
};


#endif //ARDUINO_SPI_SLAVE_SPISLAVE_H
