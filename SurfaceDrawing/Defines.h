#pragma once


#define     GRIDSIZE                75
#define     CELLSIZE                16
#define     SPEED                   20          // ms for 1 pixel
#define     CLOCKTIME               108000000   // 30 min
#define     SPAWNINGAREAREQUIRED    5           // 5x5 ( spawns are 3x3, here we let a little room )
#define     ROLLBACKSPEED           2           // Multiplier of speed
#define     SPEEDBOOST              3           // Multiplier of speed
#define     RENDERDELAY             2           // In ticks

#define     UICOMPSIZE              32
#define     UICOMPSPACING           5
#define     UICOMPCOUNT             2
#define     UIBARHEIGHT             40
#define     UIBARSPACING            10
//#define     UIBARWIDTH              (UICOMPCOUNT * UICOMPSIZE   +   (UICOMPCOUNT-1) * UICOMPSPACING   +   UIBARSPACING * 2)
#define     UIBARWIDTH              200