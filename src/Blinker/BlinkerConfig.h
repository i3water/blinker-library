#ifndef BlinkerConfig_H
#define BlinkerConfig_H

#define BLINKER_VERSION                 "0.0.1"

#define BLINKER_CONNECT_TIMEOUT_MS      10000UL

#define BLINKER_STREAM_TIMEOUT          10

#define BLINKER_MAX_WIDGET_SIZE         16

#define BLINKER_OBJECT_NOT_AVAIL        -1

#ifndef BLINKER_BUFFER_SIZE
#define BLINKER_BUFFER_SIZE             256
#endif

#define  BLINKER_NO_LOGO

//#define BLINKER_LOGO_3D

// #define	BLINKER_DEBUG

#define BLINKER_CMD_ON                  "on"

#define BLINKER_CMD_OFF                 "off"

#define BLINKER_CMD_JOYSTICK            "joy"

#define BLINKER_CMD_GYRO                "gyro"

#define BLINKER_CMD_AHRS                "ahrs"

#define BLINKER_CMD_VIBRATE             "vibrate"

#define BLINKER_CMD_BUTTON_PRESSED      "Pressed"

#define BLINKER_CMD_BUTTON_RELEASED     "Released"

#define BLINKER_CMD_NEWLINE             "\n"

#define BLINKER_JOYSTICK_VALUE_DEFAULT  128

#endif