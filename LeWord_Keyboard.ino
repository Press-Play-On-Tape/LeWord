#include <Arduboy2.h>   
#include <ArduboyFX.h>   
#include "fxdata/Images.h"

void drawKeyboard(uint8_t xOffset, uint8_t yOffset) {

                                  // A  B  C  D  E  F  G  H  I  J  K  L  M  N  O   P  Q  R  S  T  U  V  Q  X  Y  Z
    constexpr uint8_t key_PosX[] = { 1, 6, 4, 3, 3, 4, 5, 6, 8, 7, 8, 9, 8, 7, 9, 10, 1, 4, 2, 5, 7, 5, 2, 3, 6, 2 }; 
    constexpr uint8_t key_PosY[] = { 2, 3, 3, 2, 1, 2, 2, 2, 1, 2, 2, 2, 3, 3, 1,  1, 1, 1, 2, 1, 1, 3, 1, 3, 1, 3 }; 

    constexpr uint8_t key_Width = 10;
    constexpr uint8_t key_Height = 11;

    arduboy.fillRect(xOffset - 1, yOffset + 1, 93, 50, BLACK);

    for (uint8_t i = 0; i < 26; i++) {

        uint8_t xPos = key_PosX[i] - 1;
        uint8_t yPos = key_PosY[i] - 1;

        uint8_t x = xPos * (key_Width - 1) + (key_PosY[i] >= 2 ? 4 : 0) + (key_PosY[i] == 3 ? -2 : 0);
        uint8_t y = yPos * (key_Height - 1); 

        if (xPos == gamePlayVars.keyboard.xCursor && yPos == gamePlayVars.keyboard.yCursor && gamePlayVars.keyboard.state == KeyboardState::Showing) {

            FX::drawBitmap(x + xOffset, y + 1 + yOffset, Images::Keyboard_B[i], 0, dbmWhite);

            if (gamePlayVars.keyboard.keys[Constants::key_Map[gamePlayVars.keyboard.yCursor][gamePlayVars.keyboard.xCursor]] != KeyState::Invisible) {
                FX::drawBitmap(x + xOffset, y + 1 + yOffset, Images::Keyboard_B_Cursor, 0, dbmMasked);
            }
            else {
                FX::drawBitmap(x + xOffset, y + 1 + yOffset, Images::Keyboard_B_Cursor_Half, 0, dbmMasked);
            }

        }
        else {

            if (gamePlayVars.keyboard.keys[i] == KeyState::Visible) {
                FX::drawBitmap(x + xOffset, y + 1 + yOffset, Images::Keyboard_W[i], 0, dbmWhite);
            }
            else {
                FX::drawBitmap(x + xOffset, y + 1 + yOffset, Images::Keyboard_B[i], 0, dbmWhite);
            }

        }

    }


    // Delete key ..

    if (gamePlayVars.keyboard.xCursor == 0 && gamePlayVars.keyboard.yCursor == 2) {
        FX::drawBitmap(xOffset, 2 * (key_Height - 1) + 1 + yOffset, Images::Keyboard_B[26], 0, dbmWhite);
    }
    else {
        FX::drawBitmap(xOffset, 2 * (key_Height - 1) + 1 + yOffset, Images::Keyboard_W[26], 0, dbmWhite);
    }


    // Enter Key ..

    if (gamePlayVars.keyboard.xCursor == 8 && gamePlayVars.keyboard.yCursor == 2) {
        FX::drawBitmap(7 * (key_Width - 1) + (key_Width - 1) + xOffset + 2, 2 * (key_Height - 1) + 1 + yOffset, Images::Keyboard_B[27], 0, dbmWhite);
    }
    else {
        FX::drawBitmap(7 * (key_Width - 1) + (key_Width - 1) + xOffset + 2, 2 * (key_Height - 1) + 1 + yOffset, Images::Keyboard_W[27], 0, dbmWhite);
    }

    arduboy.drawFastHLine(xOffset, yOffset, 91);
    arduboy.drawFastVLine(xOffset - 1, yOffset + 1, 50);
    arduboy.drawFastVLine(xOffset + 91, yOffset + 1, 50);

    switch (gamePlayVars.keyboard.state) {

        case KeyboardState::Showing:
        case KeyboardState::StartShowing:
            FX::drawBitmap(xOffset + 86, yOffset - 4, Images::Arrow_Up, 0, dbmWhite);
            break;

        case KeyboardState::Hiding:
        case KeyboardState::StartHiding:
            FX::drawBitmap(xOffset + 86, yOffset - 4, Images::Arrow_Down, 0, dbmWhite);
            break;

    }

}