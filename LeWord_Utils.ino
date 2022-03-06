#include <Arduboy2.h>  
#include <ArduboyFX.h> 

void resetKeyboard() {

    for (uint8_t i = 0; i < 26; i++) {
        keyboard[i] = KeyState::Visible;
    }
}

void resetGuesses() {

    for (uint8_t y = 0; y < 6; y++) {
        for (uint8_t x = 0; x < 5; x++) {
            guess_Char[y][x] = ' ';
            guess_State[y][x] = GuessState::Dashed;
        }
    }

    guess_CursorX = 0;
    guess_CursorY = 0;
    guess_ListY = 0;

}

void moveCursor(Direction direction) {

    switch (direction) {

        case Direction::Right:
    
            switch (yCursor) {

                case 0:
                    if (xCursor < 9) {
                        
                        xCursor++;

                    }
                    break;

                case 1:
                case 2:
                    if (xCursor < 8) {
                        
                        xCursor++;

                    }
                    break;

            }

            break;

        case Direction::Left:

            if (xCursor > 0) {
                xCursor--;
            }

            break;

        case Direction::Down:
    
            switch (yCursor) {

                case 0:
                    if (xCursor == 9) {
                        
                        yCursor++;
                        xCursor--;

                    }
                    else {
                        yCursor++;
                    }
                    break;

                case 1:
                    yCursor++;
                    break;

            }

            break;

        case Direction::Up:
    
            if (yCursor == 0) {

                keyboardState = KeyboardState::StartHiding;
                break;

            }
            else {

                switch (yCursor) {

                    case 0:
                    case 2:
                        if (xCursor == 9) {
                            yCursor--;
                            xCursor--;
                        }
                        else {
                            yCursor--;
                        }
                        break;

                    case 1:
                        yCursor--;
                        break;


                }


            }

            break;


    }
    
}

CheckState checkWord() {

    uint8_t guessWord[5];
    uint8_t testWord[5];
    uint8_t answer[5];
    
    for (uint8_t i = 0; i < 5; i++) {
        guessWord[i] = guess_Char[guess_CursorY][i];
        answer[i] = selectedWord[i];
        guess_State[guess_CursorY][i] = GuessState::Incorrect;
    }


    // Is the word correct?

    bool isCorrectWord = true;

    for (uint8_t i = 0; i < 5; i++) {
        if (guessWord[i] != answer[i]) {
            isCorrectWord = false;
        }
    }
    
    if (isCorrectWord) {

        for (uint8_t i = 0; i < 5; i++) {
            guess_State[guess_CursorY][i] = GuessState::Correct; 
        }

        return CheckState::CorrectWord;
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

            
            // Correct letters ..

            for (uint8_t i = 0; i < 5; i++) {

                if (guessWord[i] == answer[i]) { 
                    guess_State[guess_CursorY][i] = GuessState::Correct; 
                    answer[i] = ' ';
                }

            }
// Serial.print("Z ");
//             for (uint8_t i = 0; i < 5; i++) {

// Serial.print(" ");
// Serial.print((uint8_t)guess_State[guess_CursorY][i]);

//             }
// Serial.println(" ");
            for (uint8_t i = 0; i < 5; i++) {

                if (guessWord[i] != selectedWord[i]) { 

                    for (uint8_t j = 0; j < 5; j++) {

                        if (guess_State[guess_CursorY][j] != GuessState::Correct && testWord[i] == answer[j]) { 

                            guess_State[guess_CursorY][i] = GuessState::WrongPosition;
                            answer[j] = ' ';
                            break;
                    
                        }
                    
                    }


                }

            }

// Serial.print("A ");
//             for (uint8_t i = 0; i < 5; i++) {

// Serial.print(" ");
// Serial.print((uint8_t)guess_State[guess_CursorY][i]);

//             }
// Serial.println(" ");

            // If the letter is not found but already exists, we cannot remove it from the keyboard ..

            for (uint8_t i = 0; i < 5; i++) {

                if (guess_State[guess_CursorY][i] == GuessState::Incorrect) {
// Serial.println(i);
                    for (uint8_t j = 0; j < 5; j++) {
// Serial.print(j);
// Serial.print(" ");
// Serial.print((uint8_t)guess_State[guess_CursorY][j]);
// Serial.print(" != ");
// Serial.print(1);
// Serial.print(" && ");
// Serial.print(guessWord[i]);
// Serial.print(" == ");
// Serial.print(selectedWord[j]);
// Serial.print(", ");

                        // if (guess_State[guess_CursorY][j] == GuessState::Incorrect && guessWord[i] == selectedWord[j]) { 
                        if (guessWord[i] == selectedWord[j]) { 

                            guess_State[guess_CursorY][i] = GuessState::Duplicate;
                    
                        }
                    
                    }
// Serial.println("");

                }

            }

// Serial.print("B ");
//             for (uint8_t i = 0; i < 5; i++) {

// Serial.print(" ");
// Serial.print((uint8_t)guess_State[guess_CursorY][i]);

//             }
// Serial.println(" ");
            // Disable the keys for letters not found in solution ..

            for (uint8_t i = 0; i < 5; i++) {

                if (guess_State[guess_CursorY][i] == GuessState::Incorrect) { 

                    keyboard[guessWord[i] - 65] = KeyState::Invisible;
            
                }

            }


            // Clear duplicate marks ..
// Serial.print("C ");

            for (uint8_t j = 0; j < 5; j++) {

// Serial.print(" ");
// Serial.print((uint8_t)guess_State[guess_CursorY][j]);

                if (guess_State[guess_CursorY][j] == GuessState::Duplicate) { 

                    guess_State[guess_CursorY][j] = GuessState::Incorrect;
            
                }
            
            }
// Serial.println(" ");
            return CheckState::RealWord;

        }

    }

    FX::readEnd();

}


