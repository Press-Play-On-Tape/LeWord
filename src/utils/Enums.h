#pragma once

enum GameState : uint8_t {
	SplashScreen_Init,
	SplashScreen,
	Title_Init,
	Title,
	Game_Init,
	Game,
    Stats_Init,
    Stats
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
    Normal,
    InvalidWord,
    CorrectWord,
    RealWord,
    TooManyAttempts,
    Quit,
};

enum KeyboardState : uint8_t {
    Showing,
    StartShowing,
    StartHiding,
    Hiding
};
