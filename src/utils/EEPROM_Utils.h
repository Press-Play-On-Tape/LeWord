#pragma once

#include "Arduboy2Ext.h"
#include "Enums.h"

class EEPROM_Utils {

    public: 

        EEPROM_Utils() {};
        
        static void initEEPROM(bool forceClear, GameMode gameMode, bool clearBoth);
        static void increaseCorrectWords(GameMode gameMode);
        static void resetWiningStreak(GameMode gameMode);
        static void setMode(GameMode gameMode);
        static GameMode getMode();

};


/* ----------------------------------------------------------------------------
 *   Is the EEPROM initialised?
 */
void EEPROM_Utils::initEEPROM(bool forceClear, GameMode gameMode, bool clearBoth) {

    byte c1 = eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::EEPROM_Char1));
    byte c2 = eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::EEPROM_Char2));

    uint16_t zero = 0;

    if (forceClear || c1 != 'L' || c2 != 'E') { 

        eeprom_update_byte(reinterpret_cast<uint8_t *>(Constants::EEPROM_Char1), 'L');
        eeprom_update_byte(reinterpret_cast<uint8_t *>(Constants::EEPROM_Char2), 'E');
        eeprom_update_byte(reinterpret_cast<uint8_t *>(Constants::EEPROM_Mode), static_cast<uint8_t>(gameMode));

        if (forceClear || gameMode == GameMode::English || clearBoth) { 

            EEPROM.put(Constants::EEPROM_Games_Won_EN, zero);
            EEPROM.put(Constants::EEPROM_Games_Played_EN, zero);
            EEPROM.put(Constants::EEPROM_Current_Streak_EN, zero);
            EEPROM.put(Constants::EEPROM_Max_Streak_EN, zero);

        }

        if (forceClear || gameMode == GameMode::French || clearBoth) { 

            EEPROM.put(Constants::EEPROM_Games_Won_FR, zero);
            EEPROM.put(Constants::EEPROM_Games_Played_FR, zero);
            EEPROM.put(Constants::EEPROM_Current_Streak_FR, zero);
            EEPROM.put(Constants::EEPROM_Max_Streak_FR, zero);

        }

    }

}

void EEPROM_Utils::increaseCorrectWords(GameMode gameMode) {

    uint16_t gamesWon = 0;
    uint16_t gamesPlayed = 0;
    uint16_t currentStreak = 0;
    uint16_t maxStreak = 0;

    if (gameMode == GameMode::English) {

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

    gamesWon++;
    gamesPlayed++;
    currentStreak++;

    if (currentStreak > maxStreak) maxStreak = currentStreak;

    if (gameMode == GameMode::English) {

        EEPROM.put(Constants::EEPROM_Games_Won_EN, gamesWon);
        EEPROM.put(Constants::EEPROM_Games_Played_EN, gamesPlayed);
        EEPROM.put(Constants::EEPROM_Current_Streak_EN, currentStreak);
        EEPROM.put(Constants::EEPROM_Max_Streak_EN, maxStreak);

    }
    else {

        EEPROM.put(Constants::EEPROM_Games_Won_FR, gamesWon);
        EEPROM.put(Constants::EEPROM_Games_Played_FR, gamesPlayed);
        EEPROM.put(Constants::EEPROM_Current_Streak_FR, currentStreak);
        EEPROM.put(Constants::EEPROM_Max_Streak_FR, maxStreak);

    }

}

void EEPROM_Utils::resetWiningStreak(GameMode gameMode) {

    uint16_t gamesPlayed = 0;
    uint16_t currentStreak = 0;

    if (gameMode == GameMode::English) {

        EEPROM.get(Constants::EEPROM_Games_Played_EN, gamesPlayed);
        gamesPlayed++;
        EEPROM.put(Constants::EEPROM_Games_Played_EN, gamesPlayed);
        EEPROM.put(Constants::EEPROM_Current_Streak_EN, currentStreak);

    }
    else {

        EEPROM.get(Constants::EEPROM_Games_Played_FR, gamesPlayed);
        gamesPlayed++;
        EEPROM.put(Constants::EEPROM_Games_Played_FR, gamesPlayed);
        EEPROM.put(Constants::EEPROM_Current_Streak_FR, currentStreak);

    }

}

void EEPROM_Utils::setMode(GameMode gameMode) {

    eeprom_update_byte(reinterpret_cast<uint8_t *>(Constants::EEPROM_Mode), static_cast<uint8_t>(gameMode));

}

GameMode EEPROM_Utils::getMode() {

    return static_cast<GameMode>(eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::EEPROM_Mode)));

}
