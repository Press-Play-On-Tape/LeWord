#include <Arduboy2.h>  
#include <ArduboyFX.h> 

void moveCursor(Direction direction) {

    switch (direction) {

        case Direction::Right:
    
            if (xCursor == 9) break;

            for (uint8_t i = xCursor + 1; i < 10; i++) {

                uint8_t idx = Constants::key_Map[yCursor][i];

                switch (idx) {

                    case 26:
                    case 27:
                        xCursor = i;
                        return;

                    case 255:
                        return;

                    default:

                        if (keyboard[idx] == KeyState::Visible) {

                            xCursor = i;
                            return;

                        }

                        break;
                    
                }

            }

            break;

        case Direction::Left:

            if (xCursor == 0) break;
          
            for (int8_t i = xCursor - 1; i >= 0; i--) {

                uint8_t idx = Constants::key_Map[yCursor][i];

                switch (idx) {

                    case 26:
                    case 27:
                        xCursor = i;
                        return;

                    case 255:
                        return;

                    default:

                        if (keyboard[idx] == KeyState::Visible) {

                            xCursor = i;
                            return;

                        }

                        break;
                    
                }

            }

            break;

        case Direction::Down:
    
            if (yCursor == 2) break;

            for (uint8_t i = yCursor + 1; i < 3; i++) {

                uint8_t idx = Constants::key_Map[i][xCursor];

                switch (idx) {

                    case 26:
                    case 27:
                        yCursor = i;
                        return;

                    case 255:
                        break;

                    default:

                        if (keyboard[idx] == KeyState::Visible) {

                            yCursor = i;
                            return;

                        }

                }

                if (xCursor > 0) {

                    idx = Constants::key_Map[i][xCursor - 1];

                    switch (idx) {

                        case 26:
                        case 27:
                            yCursor = i;
                            xCursor = xCursor - 1;
                            return;

                        case 255:
                            return;

                        default:

                            if (keyboard[idx] == KeyState::Visible) {

                                yCursor = i;
                                xCursor = xCursor - 1;
                                return;

                            }

                    }

                }

            }

            break;

        case Direction::Up:
    
            if (yCursor == 0) {

                keyboardState = KeyboardState::StartHiding;
                break;

            }

            for (int8_t i = yCursor - 1; i >= 0; i--) {

                uint8_t idx = Constants::key_Map[i][xCursor];

                switch (idx) {

                    case 26:
                    case 27:
                        yCursor = i;
                        return;

                    case 255:
                        break;

                    default:

                        if (keyboard[idx] == KeyState::Visible) {

                            yCursor = i;
                            return;

                        }

                }

                if ((yCursor == 1 && xCursor > 0) || (yCursor == 2 && xCursor < 9)) {

                    idx = Constants::key_Map[i][xCursor + (i == 0 ? 1 : -1)];

                    switch (idx) {

                        case 26:
                        case 27:
                            yCursor = i;
                            xCursor = xCursor + (i == 0 ? 1 : -1);
                            return;

                        case 255:
                            return;

                        default:

                            if (keyboard[idx] == KeyState::Visible) {

                                yCursor = i;
                                xCursor = xCursor + (i == 0 ? 1 : -1);
                                return;

                            }

                    }

                }

            }

            break;


    }
    
}

CheckState checkWord() {

    uint8_t guessWord[5];
    uint8_t testWord[5];
    
    for (uint8_t i = 0; i < 5; i++) {
        guessWord[i] = guess_Char[guess_CursorY][i];
    }

    // Get start of words using char one and two of guess ..

    uint24_t startPos = ((guessWord[0] - 65) * 26) + (guessWord[1] - 65);
    FX::seekData(alphaMap + (startPos * 2));
    uint16_t alphaStart = FX::readPendingUInt16();
    FX::readEnd();


    // Iterate through words looking for a match ..
    
    FX::seekData(words + (alphaStart * 6));

    while (true) {

        uint8_t status = FX::readPendingUInt8();

        for (uint8_t i = 0; i < 5; i++) {
            testWord[i] = FX::readPendingUInt8();
        }

        if (testWord[0] > guessWord[0] || (testWord[0] == guessWord[0] && testWord[1] > guessWord[1])) {
            
            FX::readEnd();
            return CheckState::InvalidWord;

        }


        // Valid word!  What are correct?

        bool isValid = true;

        for (uint8_t i = 0; i < 5; i++) {

            if (testWord[i] != guessWord[i]) {
                isValid = false;
                break;
            };

        }

        if (isValid) {

            for (uint8_t i = 0; i < 5; i++) {

                guess_State[guess_CursorY][i] = GuessState::Incorrect; 

                if (guessWord[i] == selectedWord[i]) { 
                    guess_State[guess_CursorY][i] = GuessState::Correct; 
                }

            }

            for (uint8_t i = 0; i < 5; i++) {

                if (guessWord[i] != selectedWord[i]) { 

                    for (uint8_t j = 0; j < 5; j++) {

                        if (guess_State[guess_CursorY][j] != GuessState::Correct && testWord[i] == selectedWord[j]) { 

                            guess_State[guess_CursorY][i] = GuessState::WrongPosition;
                    
                        }
                    
                    }


                }

            }


            // If the letter is not found but already exists, we cannot remove it from the keyboard ..

            for (uint8_t i = 0; i < 5; i++) {

                if (guess_State[guess_CursorY][i] == GuessState::Incorrect) {

                    for (uint8_t j = 0; j < 5; j++) {

                        if (guess_State[guess_CursorY][j] != GuessState::Incorrect && guessWord[i] == selectedWord[j]) { 

                            guess_State[guess_CursorY][i] = GuessState::Duplicate;
                    
                        }
                    
                    }

                }

            }



            // Disable the keys for letters not found in solution ..

            for (uint8_t i = 0; i < 5; i++) {

                if (guess_State[guess_CursorY][i] == GuessState::Incorrect) { 

                    keyboard[guessWord[i] - 65] = KeyState::Invisible;
            
                }

            }

            return CheckState::RealWord;

        }

    }

    FX::readEnd();

}


