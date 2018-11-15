#pragma once


#define     GRIDSIZE                75
#define     CELLSIZE                16
#define     SPEED                   20          // ms for 1 pixel
#define     CLOCKTIME               108000000   // 30 min
#define     SPAWNINGAREAREQUIRED    5           // 5x5 ( spawns are 3x3, here we let a little room )
#define     ROLLBACKSPEED           2           // Multiplier of speed
#define     SPEEDBOOST              3           // Multiplier of speed
#define     RENDERDELAY             3           // In ticks

#define     UI_COMP_SIZE            32
#define     UI_COMP_SPACING         5
#define     UI_COMP_COUNT           2
#define     UI_BAR_HEIGHT           40
#define     UI_BAR_SPACING          10
//#define     UI_BAR_WIDTH            (UI_COMP_COUNT * UI_COMP_SIZE   +   (UI_COMP_COUNT-1) * UI_COMP_SPACING   +   UI_BAR_SPACING * 2)
#define     UI_BAR_WIDTH            200


#define     UI_STAT_ICON_SIZE       16
#define     UI_STAT_TEXT_OFFSET     3
#define     UI_STAT_TEXT_WIDTH      61
#define     UI_STAT_ITEM_WIDTH      (UI_STAT_ICON_SIZE + UI_STAT_TEXT_OFFSET + UI_STAT_TEXT_WIDTH)
#define     UI_STAT_ITEM_HEIGHT     (UI_STAT_ICON_SIZE)
