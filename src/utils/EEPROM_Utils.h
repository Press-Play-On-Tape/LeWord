#pragma once

#include "Arduboy2Ext.h"
#include "Enums.h"

class EEPROM_Utils {

    public: 

        EEPROM_Utils() {};
        
        static void initEEPROM(bool forceClear);
        static void increaseCorrectWords();
        static void resetWiningStreak();

};


/* ----------------------------------------------------------------------------
 *   Is the EEPROM initialised?
 *
 *   Looks for the characters 'L' and 'W' in the first two bytes of the EEPROM
 *   memory range starting from byte EEPROM_STORAGE_SPACE_START.  If not found,
 *   it resets the settings ..
 */
void EEPROM_Utils::initEEPROM(bool forceClear) {
Serial.println("reset?");

    byte c1 = eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::EEPROM_Char1));
    byte c2 = eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::EEPROM_Char2));

    uint16_t zero = 0;
Serial.println("reset?");

    if (forceClear || c1 != 'L' || c2 != 'X') { 

        eeprom_update_byte(reinterpret_cast<uint8_t *>(Constants::EEPROM_Char1), 'L');
        eeprom_update_byte(reinterpret_cast<uint8_t *>(Constants::EEPROM_Char2), 'X');
        EEPROM.update(Constants::EEPROM_Games_Played, zero);
        EEPROM.update(Constants::EEPROM_Current_Streak, zero);
        EEPROM.update(Constants::EEPROM_Max_Streak, zero);
Serial.println("reset");
    }

}

void EEPROM_Utils::increaseCorrectWords() {

    uint16_t gamesPlayed = 0;
    uint16_t currentStreak = 0;
    uint16_t maxStreak = 0;

    EEPROM.get(Constants::EEPROM_Games_Played, gamesPlayed);
    EEPROM.get(Constants::EEPROM_Current_Streak, currentStreak);
    EEPROM.get(Constants::EEPROM_Max_Streak, maxStreak);

    gamesPlayed++;
    currentStreak++;

    if (currentStreak > maxStreak) maxStreak = currentStreak;

    EEPROM.update(Constants::EEPROM_Games_Played, gamesPlayed);
    EEPROM.update(Constants::EEPROM_Current_Streak, currentStreak);
    EEPROM.update(Constants::EEPROM_Max_Streak, maxStreak);
Serial.println("increaseCorrectWords");

}

void EEPROM_Utils::resetWiningStreak() {

    uint16_t gamesPlayed = 0;
    uint16_t currentStreak = 0;

    EEPROM.update(Constants::EEPROM_Games_Played, gamesPlayed);
    EEPROM.update(Constants::EEPROM_Current_Streak, currentStreak);

}




