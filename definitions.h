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



#define PGRAPH_ID 0x01
#define FGRAPH_ID 0x02
#define VGRAPH_ID 0x04

#define PMAXVAL 5.0
#define FMAXVAL 5.0
#define VMAXVAL 2.5

#define PGRAPHRES 4096
#define FGRAPHRES 4096
#define VGRAPHRES 2048

#define PPEAK 0x0100
#define PMEAN 0x0101
#define PEEP 0x0102
#define RR 0x0103
#define O2 0x0104
#define MVE 0x0105
#define VTI 0x0106
#define VTE 0x0107

#define SELECTION_I 0x0108
#define SELECTION_E 0x0109

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
