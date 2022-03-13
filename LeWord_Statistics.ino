#include "src/utils/Arduboy2Ext.h"

void statistics_Init() {

    gameState = GameState::Stats;

}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void statistics() { 

    uint16_t gamesWon = 0;
    uint16_t gamesPlayed = 0;
    uint16_t currentStreak = 0;
    uint16_t maxStreak = 0;
    uint16_t percent = 0;

    if (gamePlayVars.mode == GameMode::English) {

        EEPROM.get(Constants::EEPROM_Games_Won_EN, gamesWon);
        EEPROM.get(Constants::EEPROM_Games_Played_EN, gamesPlayed);
        EEPROM.get(Constants::EEPROM_Current_Streak_EN, currentStreak);
        EEPROM.get(Constants::EEPROM_Max_Streak_EN, maxStreak);

    }
    else {

        EEPROM.get(Constants::EEPROM_Games_Won_FR, gamesWon);
        EEPROM.get(Constants::EEPROM_Games_Played_FR, gamesPlayed);
        EEPROM.get(Constants::EEPROM_Current_Streak_FR, currentStreak);
        EEPROM.get(Constants::EEPROM_Max_Streak_FR, maxStreak);

    }

    percent = (gamesPlayed == 0 ? 0 : (gamesWon * 100) / gamesPlayed);

    if (arduboy.justPressed(A_BUTTON)) {
        
        gameState = GameState::Title_Init; 

    }



    // Press B to reset ..

    if (arduboy.pressed(B_BUTTON)) {

        gamePlayVars.cancelButton++;

        if (gamePlayVars.cancelButton == 64) {
            EEPROM_Utils::initEEPROM(true, gamePlayVars.mode, false);
        }

    }
    else {

        gamePlayVars.cancelButton = false;

    }

    if (gamePlayVars.mode == GameMode::English) {
        FX::drawBitmap(0, 0, Images::Statistics_EN, 0, dbmNormal);
    }
    else {
        FX::drawBitmap(0, 0, Images::Statistics_FR, 0, dbmNormal);
    }

    arduboy.setCursor(31, 14);
    if (gamesPlayed < 100) { arduboy.print("0"); }
    if (gamesPlayed < 10)  { arduboy.print("0"); }
    arduboy.print(gamesPlayed);

    arduboy.setCursor(84, 14);
    if (percent < 100) { arduboy.print("0"); }
    if (percent < 10)  { arduboy.print("0"); }
    arduboy.print(percent);

    arduboy.setCursor(31, 39);
    if (currentStreak < 100) { arduboy.print("0"); }
    if (currentStreak < 10)  { arduboy.print("0"); }
    arduboy.print(currentStreak);

    arduboy.setCursor(84, 39);
    if (maxStreak < 100) { arduboy.print("0"); }
    if (maxStreak < 10)  { arduboy.print("0"); }
    arduboy.print(maxStreak);

}
