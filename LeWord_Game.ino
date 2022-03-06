
#include <Arduboy2.h>     // required to build for Arduboy
#include <ArduboyFX.h>    // required to access the FX external flash
#include "fxdata/Images.h"
#include "src/utils/Constants.h"


void game_Init() {

    checkState = CheckState::Normal;
    gameState = GameState::Game;
    cancelButton = 0;
    
    resetKeyboard();
    resetGuesses();
    

    // Pick a random word ..

    #ifndef USE_BRINE

        // uint16_t alphaStart = 0;

        // while (true) {

        //     uint16_t rndWord = (random(0, 26) * 26) + random(0, 26);
        //     FX::seekData(alphaMap + (rndWord * 2));
        //     alphaStart = FX::readPendingUInt16();

        //     if (alphaStart != 65535) break;
        // }

        // FX::readEnd();


        // FX::seekData(words + ((alphaStart + random(0, 100)) * 6));

        // uint8_t status = FX::readPendingUInt8();

        // for (uint8_t i = 0; i < 5; i++) {
        //     selectedWord[i] = FX::readPendingUInt8();
        //     Serial.print(selectedWord[i]);
        //     Serial.print(" ");
        // }
        //     Serial.println("");

        // FX::readEnd();

        uint24_t numberOfWords = (endOfWordData - words) / 6;

        FX::seekData(words + (random(0, numberOfWords) * 6));

        uint8_t status = FX::readPendingUInt8();

        for (uint8_t i = 0; i < 5; i++) {
            selectedWord[i] = FX::readPendingUInt8();
            Serial.print(selectedWord[i]);
            Serial.print(" ");
        }
            Serial.println("");

        FX::readEnd();

    #endif

//  selectedWord[0] = 'H';
//  selectedWord[1] = 'O';
//  selectedWord[2] = 'R';
//  selectedWord[3] = 'S';
//  selectedWord[4] = 'E';

//  selectedWord[0] = 'T';
//  selectedWord[1] = 'H';
//  selectedWord[2] = 'E';
//  selectedWord[3] = 'M';
//  selectedWord[4] = 'E';
}

void game() {


    // Press B to quit ..

    if (arduboy.pressed(B_BUTTON)) {

        cancelButton++;

        if (cancelButton == 64) {
            checkState = CheckState::Quit;
        }

    }
    else {

        cancelButton = false;

    }


    switch (checkState) {

        case CheckState::InvalidWord:
            if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON))  checkState = CheckState::Normal;
            break;

        case CheckState::CorrectWord:
        case CheckState::TooManyAttempts:
        case CheckState::Quit:
            if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON))  gameState = GameState::Title_Init;
            break;

        default:

            if (arduboy.justPressed(LEFT_BUTTON))   moveCursor(Direction::Left);
            if (arduboy.justPressed(RIGHT_BUTTON))  moveCursor(Direction::Right);

            if (arduboy.pressed(UP_BUTTON)) {
                
                switch (keyboardState) {

                    case KeyboardState::Showing:
                    case KeyboardState::StartHiding:
                    case KeyboardState::StartShowing:
                        break;

                    default:

                        if (guess_ListY < 0) {
                            guess_ListY = guess_ListY + 1;
                        }
                        break;

                }

            }        

            if (arduboy.justPressed(UP_BUTTON)) {
                
                switch (keyboardState) {

                    case KeyboardState::Showing:
                        moveCursor(Direction::Up);
                        break;

                    default:
                        break;

                }

            }        

            if (arduboy.pressed(DOWN_BUTTON))   {
                
                switch (keyboardState) {

                    case KeyboardState::Showing:
                    case KeyboardState::StartHiding:
                    case KeyboardState::StartShowing:
                        break;

                    default:

                        if (guess_ListY > Constants::scroll_Limits[guess_CursorY]) {
                            guess_ListY = guess_ListY - 1;
                        }
                        else{
                            keyboardState = KeyboardState::StartShowing;
                        }

                        break;

                }

            }

            if (arduboy.justPressed(DOWN_BUTTON))   {
                
                switch (keyboardState) {

                    case KeyboardState::Showing:
                        moveCursor(Direction::Down);
                        break;

                    default:
                        break;

                }

            }


            if (arduboy.justPressed(A_BUTTON)) {

                if (xCursor == 0 && yCursor == 2) {

                    if (guess_CursorX > 0) {

                        guess_CursorX--;
                        guess_Char[guess_CursorY][guess_CursorX] = ' ';

                    }

                }
                else if (xCursor == 8 && yCursor == 2) {

                    checkState = checkWord();

                    switch (checkState) {

                        case CheckState::CorrectWord:
                            showInvalidWord_Count = 19;
                            break;

                        case CheckState::InvalidWord:
                            showInvalidWord_Count = 15;
                            break;

                        case CheckState::RealWord:

                            if (guess_CursorY == 5) {

                                checkState = CheckState::TooManyAttempts;

                            }
                            else {
                                
                                guess_CursorX = 0;
                                guess_CursorY = guess_CursorY + 1;

                                if (guess_CursorY >= 2) {
                                    guess_ListY = -((guess_CursorY - 1) * Constants::guess_Spacing);
                                }

                            }
                            break;

                        default: break;

                    }

                }
                else {
                    
                    if (keyboard[Constants::key_Map[yCursor][xCursor]] != KeyState::Invisible) {

                        if (guess_CursorX < 5) {

                            guess_Char[guess_CursorY][guess_CursorX] = 65 + Constants::key_Map[yCursor][xCursor];

                        }

                        if (guess_CursorX < 5) {

                            guess_CursorX++;

                            if (guess_CursorX == 5) {

                                xCursor = 8;
                                yCursor = 2;

                            }

                        }

                    }

                }

            }

            break;

    }


    drawGuesses(guess_ListY);

    switch (checkState) {

        case CheckState::InvalidWord:
            FX::drawBitmap(20, 48, InvalidWord, 0, dbmNormal);
            break;

        case CheckState::CorrectWord:
            FX::drawBitmap(20, 48, Correct, 0, dbmNormal);
            break;

        case CheckState::TooManyAttempts:
            FX::drawBitmap(20, 30, TooManyAttempts, 0, dbmNormal);
            drawSolution();
            break;

        case CheckState::Quit:
            FX::drawBitmap(20, 30, Quit, 0, dbmNormal);
            drawSolution();
            break;

        default:
            drawKeyboard(19, keyboardY);
            break;

    }


    // Show or hide keyboard ..

    switch (keyboardState) {

        case KeyboardState::StartHiding:

            keyboardY++;
            if (guess_CursorY >= 2) guess_ListY++;

            if (keyboardY == 64) {
                keyboardState = KeyboardState::Hiding;
            }
            break;

        case KeyboardState::StartShowing:

            keyboardY--;
            if (guess_CursorY >= 2) guess_ListY--;

            if (keyboardY == 33) {
                keyboardState = KeyboardState::Showing;
            }
            break;
        
        default:
            break;
    }



    // Housekeeping ..

    if (showInvalidWord_Count > 0 && arduboy.getFrameCount(2) == 0) showInvalidWord_Count--;

}


void drawSolution() {

    // Render words ..

    for (uint8_t x = 0; x < 5; x++) {

        arduboy.setCursor(Constants::guess_Left + (x * Constants::guess_Spacing) + 3, 50);
        arduboy.print(selectedWord[x]);
        arduboy.drawRect(Constants::guess_Left + (x * Constants::guess_Spacing), 50 - 2, Constants::guess_Spacing - 2, Constants::guess_Spacing - 2);

    }

}


void drawGuesses(int8_t yOffset) {

    for (uint8_t y = 0; y < guess_CursorY + 1; y++) {


        // Animate last row?

        int8_t xMove = 0;
        int8_t yMove[5] = { 0, 0, 0, 0, 0 };
        
        switch (checkState) {

            case CheckState::InvalidWord:

                if (y == guess_CursorY) {
                    if (showInvalidWord_Count > 0) {
                        xMove = Constants::shake[showInvalidWord_Count];
                    }
                }
                break;

            case CheckState::CorrectWord:

                if (y == guess_CursorY) {

                    if (showInvalidWord_Count > 0 && showInvalidWord_Count < 15) yMove[0] = Constants::shake[showInvalidWord_Count];
                    if (showInvalidWord_Count > 1 && showInvalidWord_Count < 16) yMove[1] = Constants::shake[showInvalidWord_Count - 1];
                    if (showInvalidWord_Count > 2 && showInvalidWord_Count < 17) yMove[2] = Constants::shake[showInvalidWord_Count - 2];
                    if (showInvalidWord_Count > 3 && showInvalidWord_Count < 18) yMove[3] = Constants::shake[showInvalidWord_Count - 3];
                    if (showInvalidWord_Count > 4 && showInvalidWord_Count < 19) yMove[4] = Constants::shake[showInvalidWord_Count - 4];

                }

                break;

            default: break;

        }


        // Render words ..

        for (uint8_t x = 0; x < 5; x++) {

            arduboy.setCursor(Constants::guess_Left + (x * Constants::guess_Spacing) + 3 + xMove, y * Constants::guess_Spacing + 2 + yOffset + yMove[x]);
            arduboy.print(guess_Char[y][x]);

            switch (guess_State[y][x]) {

                case GuessState::Correct:
                    arduboy.drawRect(Constants::guess_Left + (x * Constants::guess_Spacing) + xMove, y * Constants::guess_Spacing + yOffset + yMove[x], Constants::guess_Spacing - 2, Constants::guess_Spacing - 2);
                    break;

                case GuessState::WrongPosition:
                    arduboy.drawFastHLine(Constants::guess_Left + (x * Constants::guess_Spacing) + 1 + xMove, ((y + 1) * Constants::guess_Spacing) - 2 + yOffset + yMove[x], Constants::guess_Spacing - 4);
                    break;

                case GuessState::Dashed:
                    arduboy.drawPixel(Constants::guess_Left + (x * Constants::guess_Spacing) + 1 + xMove, ((y + 1) * Constants::guess_Spacing) - 2 + yOffset + yMove[x]);
                    arduboy.drawPixel(Constants::guess_Left + (x * Constants::guess_Spacing) + 3 + xMove, ((y + 1) * Constants::guess_Spacing) - 2 + yOffset + yMove[x]);
                    arduboy.drawPixel(Constants::guess_Left + (x * Constants::guess_Spacing) + 5 + xMove, ((y + 1) * Constants::guess_Spacing) - 2 + yOffset + yMove[x]);
                    arduboy.drawPixel(Constants::guess_Left + (x * Constants::guess_Spacing) + 7 + xMove, ((y + 1) * Constants::guess_Spacing) - 2 + yOffset + yMove[x]);
                    arduboy.drawPixel(Constants::guess_Left + (x * Constants::guess_Spacing) + 9 + xMove, ((y + 1) * Constants::guess_Spacing) - 2 + yOffset + yMove[x]);
                    break;


                default:
                    break;

            }

        }

    }

}

