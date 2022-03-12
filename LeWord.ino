
#include "src/utils/Arduboy2Ext.h"
#include <ArduboyFX.h>

#include "fxdata/Images.h"
#include "src/images/Images.h"
#include "src/utils/Constants.h"
#include "src/utils/Enums.h"
#include "src/utils/EEPROM_Utils.h"
#include "src/utils/Structs.h"

Arduboy2Ext arduboy;
TitleScreenVars titleScreenVars;
GamePlayVars gamePlayVars;

GameState gameState = GameState::SplashScreen_Init;

uint16_t man_delay = random(50, 400);
uint8_t man_Idx = 0;

void setup() {

    arduboy.boot();
    arduboy.flashlight();
    arduboy.systemButtons();
    arduboy.setFrameRate(60);
    arduboy.initRandomSeed();

    FX::disableOLED();  
    FX::begin(FX_DATA_PAGE); 

    EEPROM_Utils::initEEPROM(false, GameMode::English, true);
    gamePlayVars.mode = EEPROM_Utils::getMode();

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

        case GameState::Stats_Init:

            statistics_Init();
            statistics();
            break;

        case GameState::Stats:

            statistics();
            break;

    }

    FX::enableOLED();              
    arduboy.display(CLEAR_BUFFER); 
    FX::disableOLED();             

}
