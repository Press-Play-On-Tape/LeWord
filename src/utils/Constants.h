#pragma once

namespace Constants {

    constexpr uint8_t guess_Left = 30;
    constexpr uint8_t guess_Spacing = 13;

    constexpr uint8_t key_Map[3][10] = { 
        { 16, 22,  4, 17, 19, 24, 20,  8, 14,  15, },
        {  0, 18,  3,  5,  6,  7,  9, 10, 11, 255},
        { 26, 25, 23,  2, 21,  1, 13, 12, 27, 255  }
        }; 

};


    
enum class Direction : uint8_t {
    Up,
    Down,
    Left,
    Right
};

enum class KeyState : uint8_t {
    Visible,
    Invisible,
};

enum class GuessState : uint8_t {
    Dashed,
    Incorrect,
    Correct,
    WrongPosition,
    Duplicate,
};

enum class CheckState : uint8_t {
    InvalidWord,
    CorrectWord,
    RealWord,
};

enum KeyboardState : uint8_t {
    Showing,
    StartShowing,
    StartHiding,
    Hiding
};