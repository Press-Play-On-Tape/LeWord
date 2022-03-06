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

    FX::drawBitmap(0, 0, TitleScreen, 0, dbmWhite);

}
