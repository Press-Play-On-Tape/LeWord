#pragma once

#define USE_BRINE
/* TEASE PRUNE DRONE CRIME BLINK and BRIEF or BRINE */

namespace Constants {

    constexpr uint8_t guess_Left = 30;
    constexpr uint8_t guess_Spacing = 13;

    constexpr uint8_t key_Map[3][10] = { 
        { 16, 22,  4, 17, 19, 24, 20,  8, 14,  15, },
        {  0, 18,  3,  5,  6,  7,  9, 10, 11, 255},
        { 26, 25, 23,  2, 21,  1, 13, 12, 27, 255  }
        }; 

    constexpr int8_t scroll_Limits[] = { 0, 0, 18, 5, -6, -20 };

    constexpr int8_t shake[] = { 0, 1, 2, 1, 0, -1, -2, -1, 0, 1, 2, 1, 0, -1, -2, -1, };

};
