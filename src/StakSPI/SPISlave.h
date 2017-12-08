//
// Created by ryo on 2017/12/03.
//

#ifndef ARDUINO_SPI_SLAVE_SPISLAVE_H
#define ARDUINO_SPI_SLAVE_SPISLAVE_H

#include "Arduino.h"
#include "SPI.h"

typedef enum {
    STANDBY = 0x01,             //送受信可能
    RECEIVE_READY = 0x11,        //受信準備
    RECEIVE_TRANSFER = 0x12,    //受信中
    RECEIVE_COMPLETE = 0x13,    //受信完了
    SEND_STANDBY = 0x20,           //送信準備
    SEND_READY = 0x21,           //送信準備
    SEND_TRANSFER = 0x22,       //送信中
    SEND_COMPLETE = 0x23,       //送信完了
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
    char buf[1024];
    volatile byte pos;
    volatile byte send_pos;
    volatile boolean process_it;
} data_t;

class SPISlave {
public:
    void begin();

    void handler();

    void send(const char *message);

    void SLAVE_CALLBACK(char *message);

private:
};


#endif //ARDUINO_SPI_SLAVE_SPISLAVE_H
