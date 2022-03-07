#include "src/utils/Arduboy2Ext.h"

void statistics_Init() {

    gameState = GameState::Stats;

}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void statistics() { 

    uint16_t gamesPlayed = 0;
    uint16_t currentStreak = 0;
    uint16_t maxStreak = 0;

    // EEPROM.get(Constants::EEPROM_Games_Played, gamesPlayed);
    // EEPROM.get(Constants::EEPROM_Current_Streak, currentStreak);
    // EEPROM.get(Constants::EEPROM_Max_Streak, maxStreak);

    if (arduboy.justPressed(A_BUTTON)) {
        
        gameState = GameState::Title_Init; 

    }

    FX::drawBitmap(0, 0, Statistics, 0, dbmNormal);

    arduboy.setCursor(33, 2);
    arduboy.print("23");

    arduboy.setCursor(81, 2);
    arduboy.print("100");

    arduboy.setCursor(33, 36);
    arduboy.print("15");

    arduboy.setCursor(83, 36);
    arduboy.print("19");

}
