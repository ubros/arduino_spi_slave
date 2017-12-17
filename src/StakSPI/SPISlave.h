//
// Created by ryo on 2017/12/03.
//

#ifndef ARDUINO_SPI_SLAVE_SPISLAVE_H
#define ARDUINO_SPI_SLAVE_SPISLAVE_H

#include "Arduino.h"
#include "SPI.h"
#include "global.h"


typedef enum {
    STANDBY = 128,             //送受信可能
    RECEIVE_READY = 129,        //受信準備
    RECEIVE_TRANSFER = 130,    //受信中
    RECEIVE_COMPLETE = 131,    //受信完了
    SEND_STANDBY = 132,           //送信準備
    SEND_READY = 133,           //送信準備
    SEND_TRANSFER = 134,       //送信中
    SEND_COMPLETE = 135,       //送信完了
} STATUS;

typedef enum {
    ZERO = 0,                   //ゼロデータ
    SUCCESS = 0x01,             //成功
    ERROR = 0x02,               //失敗
} RESPONSE;

struct {
    STATUS status;
} status_t;

struct {
    universal_t buf[512];
    int pos;
    boolean bit_order;
    volatile byte send_pos;
    volatile boolean process_it;
} data_t;

class SPISlave {
public:
    void begin();

    void handler();

    void send(const char *message);

    void SLAVE_CALLBACK(universal_t *message, size_t length);

private:
};


#endif //ARDUINO_SPI_SLAVE_SPISLAVE_H
