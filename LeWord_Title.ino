#include "src/utils/Arduboy2Ext.h"

void title_Init() {

    gameState = GameState::Title;

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

    }

    if (arduboy.justPressed(RIGHT_BUTTON) && gamePlayVars.mode == GameMode::English) {
        
        gamePlayVars.mode = GameMode::French; 

    }

    if (arduboy.justPressed(B_BUTTON)) {
        
        gameState = GameState::Stats_Init; 

    }

    FX::drawBitmap(0, 0, TitleScreen, 0, dbmWhite);
    FX::drawBitmap(gamePlayVars.mode == GameMode::English ? 16 : 74, 47, Pointer, 0, dbmWhite);

}
