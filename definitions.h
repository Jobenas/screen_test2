#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define SENSOR1 A0
#define selector A1

#define PUSHBUTTON 4

#define ENCODERA 2
#define ENCODERB 3

#define ENCODERMAXVAL 140
#define ENCODERMINVAL 0

#define ENCDIRUP 1
#define ENCDIRDOWN 0

#define ENCWINDOW 10

#define SCREEN_SETTING_ID 0x2000
#define SCREEN_GO_ID 0x2050

uint8_t encDir;
int encoder;
int lastEncoderVal;
int selectionVal;

int settingState;
int opState;

long encWindowStart;

uint8_t screenRxBuffer[50];
uint16_t index;

#endif
