
#include <Arduboy2.h>     // required to build for Arduboy
#include <ArduboyFX.h>    // required to access the FX external flash
#include "fxdata/Images.h"
#include "src/utils/Constants.h"


KeyState keyboard[26];

char guess_Char[6][5];
GuessState guess_State[6][5];
uint8_t guess_CursorX = 0;
uint8_t guess_CursorY = 0;
int8_t guess_ListY = 0;

Arduboy2 arduboy;
uint8_t xCursor = 0;
uint8_t yCursor = 0;
uint8_t keyboardY = 33;
KeyboardState keyboardState;
bool showInvalidWord = false;

char selectedWord[5] = { 'B', 'R', 'I', 'N', 'E' };


void setup() {

    arduboy.begin();
    arduboy.setFrameRate(60);
    // arduboy.setTextSize(2); 
    FX::disableOLED();  
    FX::begin(FX_DATA_PAGE); 

    resetKeyboard();

    // guess_Char[0][0]='A';
    // guess_Char[0][1]='E';
    // guess_Char[0][2]='A';
    // guess_Char[0][3]='S';
    // guess_Char[0][4]='E';

    // guess_Char[1][0]='B';
    // guess_Char[1][1]='E';
    // guess_Char[1][2]='A';
    // guess_Char[1][3]='S';
    // guess_Char[1][4]='E';

    // guess_Char[2][0]='C';
    // guess_Char[2][1]='E';
    // guess_Char[2][2]='A';
    // guess_Char[2][3]='S';
    // guess_Char[2][4]='E';

    // guess_Char[3][0]='D';
    // guess_Char[3][1]='E';
    // guess_Char[3][2]='A';
    // guess_Char[3][3]='S';
    // guess_Char[3][4]='E';

    // guess_Char[4][0]='E';
    // guess_Char[4][1]='E';
    // guess_Char[4][2]='A';
    // guess_Char[4][3]='S';
    // guess_Char[4][4]='E';

    // guess_CursorY = 5;
    // guess_ListY = -((guess_CursorY - 1) * Constants::guess_Spacing);
    // guess_CursorY = 3;
    // guess_ListY = -((guess_CursorY - 1) * Constants::guess_Spacing);

}

void loop() {
   

    if (!arduboy.nextFrame()) return; 
    arduboy.pollButtons();

Serial.print(guess_ListY);
Serial.print(" ");
Serial.println(keyboardY);


    if (showInvalidWord) {

        if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON))  showInvalidWord = false;;


    }
    else {

        if (arduboy.justPressed(LEFT_BUTTON))   moveCursor(Direction::Left);
        if (arduboy.justPressed(RIGHT_BUTTON))  moveCursor(Direction::Right);

        if (arduboy.pressed(UP_BUTTON)) {
            
            switch (keyboardState) {

                case KeyboardState::Showing:
                case KeyboardState::StartHiding:
                case KeyboardState::StartShowing:
                    // moveCursor(Direction::Up);
                    break;

                default:

                    {
                        int8_t lowerY = -((guess_CursorY - 1) * Constants::guess_Spacing);
                        if (guess_ListY < 0) {
                            guess_ListY = guess_ListY + 1;
                        }
                    }
//                    keyboardState = KeyboardState::StartHiding;
                    break;

            }

        }        

        if (arduboy.justPressed(UP_BUTTON)) {
            
            switch (keyboardState) {

                case KeyboardState::Showing:
                    moveCursor(Direction::Up);
                    break;

                default:

                    // {
                    //     int8_t lowerY = -((guess_CursorY - 1) * Constants::guess_Spacing);
                    //     if (guess_ListY < 0) {
                    //         guess_ListY = guess_ListY + 1;
                    //     }
                    // }
//                    keyboardState = KeyboardState::StartHiding;
                    break;

            }

        }        

        if (arduboy.pressed(DOWN_BUTTON))   {
            
            switch (keyboardState) {

                case KeyboardState::Showing:
                case KeyboardState::StartHiding:
                case KeyboardState::StartShowing:
                    // moveCursor(Direction::Down);
                    break;

                default:
                    {
                        int8_t lowerY = -((guess_CursorY - 1) * Constants::guess_Spacing);
                        // if (guess_ListY > lowerY) {
                        // if (guess_ListY > -17) {
                        if (guess_ListY > Constants::scroll_Limits[guess_CursorY]) {
                            guess_ListY = guess_ListY - 1;
                        }
                        else{
                            keyboardState = KeyboardState::StartShowing;
                        }
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
                    //keyboardState = KeyboardState::StartShowing;
                    break;

            }

        }


        if (arduboy.justPressed(A_BUTTON)) {

            if (xCursor == 0 && yCursor == 2 && guess_CursorX > 0) {

                guess_CursorX--;
                guess_Char[guess_CursorY][guess_CursorX] = ' ';

            }
            else if (xCursor == 8 && yCursor == 2) {

                CheckState checkState = checkWord();

                switch (checkState) {

                    case CheckState::InvalidWord:

                        showInvalidWord = true;
                        break;

                    case CheckState::RealWord:
                        guess_CursorX = 0;
                        guess_CursorY = guess_CursorY + 1;

                        if (guess_CursorY >= 2) {
                            guess_ListY = -((guess_CursorY - 1) * Constants::guess_Spacing);
                            Serial.println(guess_ListY);
                        }
                        break;

                }

            }
            else {
                
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


    drawGuesses(guess_ListY);
    drawKeyboard(19, keyboardY);

    if (showInvalidWord) {
        FX::drawBitmap(10, 10, InvalidWord, 0, dbmMasked);
    }


    FX::enableOLED();              // only enable OLED for updating the display
    arduboy.display(CLEAR_BUFFER); // Using CLEAR_BUFFER will clear the display buffer after it is displayed
    FX::disableOLED();             // disable display again so external flash can be accessed at any time


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


}


void drawGuesses(int8_t yOffset) {

    for (uint8_t y = 0; y < guess_CursorY + 1; y++) {

        for (uint8_t x = 0; x < 5; x++) {

            arduboy.setCursor(Constants::guess_Left + (x * Constants::guess_Spacing) + 3, y * Constants::guess_Spacing + 2 + yOffset);
            arduboy.print(guess_Char[y][x]);

            switch (guess_State[y][x]) {

                case GuessState::Correct:
                    arduboy.drawRect(Constants::guess_Left + (x * Constants::guess_Spacing), y * Constants::guess_Spacing + yOffset, Constants::guess_Spacing - 2, Constants::guess_Spacing - 2);
                    break;

                case GuessState::WrongPosition:
                    arduboy.drawFastHLine(Constants::guess_Left + (x * Constants::guess_Spacing) + 1, ((y + 1) * Constants::guess_Spacing) - 2 + yOffset, Constants::guess_Spacing - 4);
                    break;

                case GuessState::Dashed:
                    arduboy.drawPixel(Constants::guess_Left + (x * Constants::guess_Spacing) + 1, ((y + 1) * Constants::guess_Spacing) - 2 + yOffset);
                    arduboy.drawPixel(Constants::guess_Left + (x * Constants::guess_Spacing) + 3, ((y + 1) * Constants::guess_Spacing) - 2 + yOffset);
                    arduboy.drawPixel(Constants::guess_Left + (x * Constants::guess_Spacing) + 5, ((y + 1) * Constants::guess_Spacing) - 2 + yOffset);
                    arduboy.drawPixel(Constants::guess_Left + (x * Constants::guess_Spacing) + 7, ((y + 1) * Constants::guess_Spacing) - 2 + yOffset);
                    arduboy.drawPixel(Constants::guess_Left + (x * Constants::guess_Spacing) + 9, ((y + 1) * Constants::guess_Spacing) - 2 + yOffset);
                    break;


                default:
                    break;

            }

        }

    }

}

