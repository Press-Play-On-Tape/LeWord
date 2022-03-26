#include "src/utils/Arduboy2Ext.h"

void statistics_Init() {

    statisticsScreenVars.reset();
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
                
        switch (statisticsScreenVars.mode) {

            case StatisticsMode::PageOne:
                statisticsScreenVars.mode = StatisticsMode::PageTwo;
                break;

            case StatisticsMode::PageTwo:
                gameState = GameState::Title_Init; 
                break;

        }

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


    switch (statisticsScreenVars.mode) {

        case StatisticsMode::PageOne:

            if (gamePlayVars.mode == GameMode::English) {
                FX::drawBitmap(0, 0, Images::Statistics_Header_EN, 0, dbmNormal);
                FX::drawBitmap(0, 8, Images::Statistics_EN, 0, dbmNormal);
            }
            else {
                FX::drawBitmap(0, 0, Images::Statistics_Header_FR, 0, dbmNormal);
                FX::drawBitmap(0, 8, Images::Statistics_FR, 0, dbmNormal);
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

            break;

        case StatisticsMode::PageTwo:

            if (gamePlayVars.mode == GameMode::English) {
                FX::drawBitmap(0, 0, Images::Statistics_Header_EN, 0, dbmNormal);
            }
            else {
                FX::drawBitmap(0, 0, Images::Statistics_Header_FR, 0, dbmNormal);
            }

            FX::drawBitmap(0, 8, Images::Statistics_Vert, 0, dbmNormal);

            for (uint8_t i = 0; i < 6; i++) {

                uint8_t width = EEPROM_Utils::getPercent(gamePlayVars.mode, i);
                uint16_t val = EEPROM_Utils::getPercentVal(gamePlayVars.mode, i);
                uint8_t textWidth = widthOfNumber(val);

                if (val != 0 && textWidth > width) width = textWidth + 1;

                if (i + 1 == statisticsScreenVars.numberOfAttempts) {
                    arduboy.drawRect(12, 15 + (i * 8), width == 0 ? 1 : width, 7);
                }
                else {
                    arduboy.fillRect(12, 15 + (i * 8), width == 0 ? 1 : width, 7);
                }


                if (val > 0) {

                    uint8_t digits[5];
                    extractDigits(digits, val);
                    bool firstDigitRendered = false;

                    for (int8_t j = 4; j >= 0; j--) {

                        if (digits[j] != 0 || firstDigitRendered) {

                            if (i + 1 == statisticsScreenVars.numberOfAttempts) {
                                if (arduboy.getFrameCountHalf(32)) {
                                    FX::drawBitmap(12 + width - (j * 4) - 5, 16 + (i * 8), Images::Numbers3x5, digits[j], dbmNormal);
                                }
                            }
                            else {
                                FX::drawBitmap(12 + width - (j * 4) - 5, 16 + (i * 8), Images::Numbers3x5, digits[j], dbmReverse);
                            }

                            firstDigitRendered = true;

                        }

                    }

                }

            }

            break;

    }

    
}

uint8_t widthOfNumber(uint16_t number) {

    if (number >= 10000) return 5 * 4 + 2;
    if (number >= 1000) return 4 * 4 + 2;
    if (number >= 100) return 3 * 4 + 2;
    if (number >= 10) return 2 * 4 + 2;
    if (number >= 1) return 1 * 4 + 2;
    return 0;

}