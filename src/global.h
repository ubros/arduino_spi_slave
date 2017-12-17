//
// Created by ryo on 2017/12/12.
//

#ifndef STAK_BASE_GLOBAL_H
#define STAK_BASE_GLOBAL_H

#define LED_ON 0x01
#define LED_OFF 0x02
#define MQTT_END_POINT "a2wogwwavwnh4x.iot.us-east-1.amazonaws.com"
#define MQTT_PORT 8883

typedef enum MQTT_TYPE {
    MQTT_TYPE_NONE = 0x00,
    MQTT_TYPE_AWS = 0x01,
    MQTT_TYPE_IR = 0x02
} MQTT_TYPE;

typedef uint8_t LED_STATUS;

union universal_t {
    char text[2];
    uint8_t bit8[2];
    uint16_t bit16;
};

#endif //STAK_BASE_GLOBAL_H
