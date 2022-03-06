
#include "src/utils/Arduboy2Ext.h"
#include <ArduboyFX.h>

#include "fxdata/Images.h"
#include "src/images/Images.h"
#include "src/utils/Constants.h"
#include "src/utils/Enums.h"


KeyState keyboard[26];

char guess_Char[6][5];
GuessState guess_State[6][5];
uint8_t guess_CursorX = 0;
uint8_t guess_CursorY = 0;
int8_t guess_ListY = 0;

Arduboy2Ext arduboy;
uint8_t xCursor = 0;
uint8_t yCursor = 0;
uint8_t keyboardY = 33;
KeyboardState keyboardState;

uint8_t showInvalidWord_Count = 0;
CheckState checkState = CheckState::Normal;
uint8_t cancelButton = 0;

#ifdef USE_BRINE
char selectedWord[5] = { 'B', 'R', 'I', 'N', 'E' };
#else
char selectedWord[5] = { ' ', ' ', ' ', ' ', ' ' };
#endif

GameState gameState = GameState::SplashScreen_Init;

void setup() {

    arduboy.boot();
    arduboy.flashlight();
    arduboy.systemButtons();
    arduboy.setFrameRate(60);
    arduboy.initRandomSeed();

    FX::disableOLED();  
    FX::begin(FX_DATA_PAGE); 

}

void loop() {
   
    if (!arduboy.nextFrame()) return; 
    arduboy.pollButtons();

    switch (gameState) {

        case GameState::SplashScreen_Init:

            splashScreen_Init();
            splashScreen();
            break;

        case GameState::SplashScreen:

            splashScreen();
            break;

        case GameState::Title_Init:

            gameState = GameState::Title;
            title_Init();
            title();
            break;

        case GameState::Title:

            title();
            break;

        case GameState::Game_Init:

            game_Init();
            game();
            break;

        case GameState::Game:

            game();
            break;

    }

    FX::enableOLED();              
    arduboy.display(CLEAR_BUFFER); 
    FX::disableOLED();             

}
