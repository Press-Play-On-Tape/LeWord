#include "src/utils/Arduboy2Ext.h"

void title_Init() {

    gameState = GameState::Title;

    titleScreenVars.delay = random(16, 48);

}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void title() { 

    if (arduboy.justPressed(A_BUTTON)) {
        
        gameState = GameState::Game_Init; 

    }

    if (arduboy.justPressed(LEFT_BUTTON) && gamePlayVars.mode == GameMode::French) {
        
        gamePlayVars.mode = GameMode::English; 
        EEPROM_Utils::setMode(gamePlayVars.mode);

    }

    if (arduboy.justPressed(RIGHT_BUTTON) && gamePlayVars.mode == GameMode::English) {
        
        gamePlayVars.mode = GameMode::French; 
        EEPROM_Utils::setMode(gamePlayVars.mode);

    }

    if (arduboy.justPressed(B_BUTTON)) {
        
        gameState = GameState::Stats_Init; 

    }

    uint8_t indexes[6] = { 0, 0, 0, 0, 0, 0, };

    if (titleScreenVars.delay == 0) {

        indexes[titleScreenVars.charIndex] = titleScreenVars.frameIndex - 1;

    }
    else {

        titleScreenVars.delay--;

        if (titleScreenVars.delay == 0) {

            titleScreenVars.charIndex = random(0, 6);
            titleScreenVars.frameIndex = 19;
            
        }

    }

    FX::drawBitmap(14, 11, Images::TitleScreen_L[indexes[0]], 0, dbmWhite);
    FX::drawBitmap(31, 11, Images::TitleScreen_E[indexes[1]], 0, dbmWhite);
    FX::drawBitmap(48, 11, Images::TitleScreen_W[indexes[2]], 0, dbmWhite);
    FX::drawBitmap(65, 11, Images::TitleScreen_O[indexes[3]], 0, dbmWhite);
    FX::drawBitmap(82, 11, Images::TitleScreen_R[indexes[4]], 0, dbmWhite);
    FX::drawBitmap(99, 11, Images::TitleScreen_D[indexes[5]], 0, dbmWhite);

    FX::drawBitmap(0, 29, TitleScreen_Lower, 0, dbmWhite);
    FX::drawBitmap(gamePlayVars.mode == GameMode::English ? 16 : 74, 47, Pointer, 0, dbmWhite);

    if (titleScreenVars.frameIndex > 0) {
    
        titleScreenVars.frameIndex--;
    
        if (titleScreenVars.frameIndex == 0) {

            titleScreenVars.delay = random(32, 200);

        }

    }

}
