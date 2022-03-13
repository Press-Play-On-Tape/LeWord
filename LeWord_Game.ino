
#include <Arduboy2.h>     // required to build for Arduboy
#include <ArduboyFX.h>    // required to access the FX external flash
#include "fxdata/Images.h"
#include "src/utils/Constants.h"


void game_Init() {

    gamePlayVars.checkState = CheckState::Normal;
    gamePlayVars.cancelButton = 0;
    gamePlayVars.keyboard.reset();
    gamePlayVars.guesses.reset();

    gameState = GameState::Game;
    

    // Pick a random word ..

    #ifndef USE_BRINE

        switch (gamePlayVars.mode) {

            case GameMode::English:
                {
                    while (true) {

                        uint24_t numberOfWords = (Dictionary::English_EndOfWordData - Dictionary::English_Words) / 6;
                        FX::seekData(Dictionary::English_Words + (random(0, numberOfWords) * 6));
                        uint8_t status = FX::readPendingUInt8();

                        for (uint8_t i = 0; i < 5; i++) {
                            gamePlayVars.selectedWord[i] = FX::readPendingUInt8();
                        }

                        FX::readEnd();

                        if (status == 1) break;

                    }
                }
                break;

            case GameMode::French:
                {
                    uint24_t numberOfWords = (Dictionary::French_EndOfWordData - Dictionary::French_Words) / 6;
                    FX::seekData(Dictionary::French_Words + (random(0, numberOfWords) * 6));

                    uint8_t status = FX::readPendingUInt8();

                    for (uint8_t i = 0; i < 5; i++) {
                        gamePlayVars.selectedWord[i] = FX::readPendingUInt8();
                    }

                    FX::readEnd();
                }
                break;

        }                

    #endif

}

void game() {

    uint8_t tmpManX = 0;
    uint8_t tmpManB = 0;
    uint8_t tmpManI = 0;
    uint8_t tmpManP = 0;
    int8_t tmpManK = 0;


    if (man_delay == 0) {

        switch (animation) {

            case AnimationSequence::Think_L:

                FX::seekData(static_cast<uint24_t>(Animation::ManData_L + (man_Idx * 5)));
                tmpManX = FX::readPendingUInt8();
                tmpManK = FX::readPendingUInt8();
                tmpManB = FX::readPendingUInt8();
                tmpManI = FX::readPendingUInt8();
                tmpManP = FX::readPendingUInt8();
                FX::readEnd();
                break;

            case AnimationSequence::Think_R: 
                
                FX::seekData(static_cast<uint24_t>(Animation::ManData_R + (man_Idx * 5)));
                tmpManX = FX::readPendingUInt8();
                tmpManK = -FX::readPendingUInt8();
                tmpManB = FX::readPendingUInt8();
                tmpManI = FX::readPendingUInt8();
                tmpManP = FX::readPendingUInt8();
                FX::readEnd();
                break;

            case AnimationSequence::Walk_LR: 

                tmpManX = man_Idx;
                tmpManK = 0;
                tmpManB = 0;
                tmpManI = ((man_Idx % 6) / 2) + 3;
                tmpManP = 0;
                break;

            case AnimationSequence::Walk_RL: 

                tmpManX = 128 + 16 - man_Idx;
                tmpManK = 0;
                tmpManB = 0;
                tmpManI = ((man_Idx % 6) / 2);
                tmpManP = 0;
                break;

        }

    }


    // Press B to quit ..

    if (arduboy.pressed(B_BUTTON)) {

        gamePlayVars.cancelButton++;

        if (gamePlayVars.cancelButton == 64) {
            gamePlayVars.checkState = CheckState::Quit;
        }

    }
    else {

        gamePlayVars.cancelButton = false;

    }


    switch (gamePlayVars.checkState) {

        case CheckState::InvalidWord:
            if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON)) {
                gamePlayVars.checkState = CheckState::Normal;
            }
            break;

        case CheckState::CorrectWord:
            if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON)) {
                EEPROM_Utils::increaseCorrectWords(gamePlayVars.mode);
                gameState = GameState::Stats_Init;
            }
            break;

        case CheckState::TooManyAttempts:
            if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON)) {
                EEPROM_Utils::resetWiningStreak(gamePlayVars.mode);
                gameState = GameState::Stats_Init;
            }
            break;

        case CheckState::Quit:
            if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON)) {
                if (gamePlayVars.guesses.yCursor > 0) EEPROM_Utils::resetWiningStreak(gamePlayVars.mode);
                gameState = GameState::Stats_Init;
            }
            break;

        default:

            if (arduboy.justPressed(LEFT_BUTTON))   moveCursor(Direction::Left);
            if (arduboy.justPressed(RIGHT_BUTTON))  moveCursor(Direction::Right);

            if (arduboy.pressed(UP_BUTTON)) {
                
                switch (gamePlayVars.keyboard.state) {

                    case KeyboardState::Showing:
                    case KeyboardState::StartHiding:
                    case KeyboardState::StartShowing:
                        break;

                    default:

                        if (gamePlayVars.guesses.listY < 0) {
                            gamePlayVars.guesses.listY = gamePlayVars.guesses.listY + 1;
                        }
                        break;

                }

            }        

            if (arduboy.justPressed(UP_BUTTON)) {
                
                switch (gamePlayVars.keyboard.state) {

                    case KeyboardState::Showing:
                        moveCursor(Direction::Up);
                        break;

                    default:
                        break;

                }

            }        

            if (arduboy.pressed(DOWN_BUTTON))   {
                
                switch (gamePlayVars.keyboard.state) {

                    case KeyboardState::Showing:
                    case KeyboardState::StartHiding:
                    case KeyboardState::StartShowing:
                        break;

                    default:

                        if (gamePlayVars.guesses.listY > Constants::scroll_Limits[gamePlayVars.guesses.yCursor]) {
                            gamePlayVars.guesses.listY = gamePlayVars.guesses.listY - 1;
                        }
                        else{
                            gamePlayVars.keyboard.state = KeyboardState::StartShowing;
                        }

                        break;

                }

            }

            if (arduboy.justPressed(DOWN_BUTTON))   {
                
                switch (gamePlayVars.keyboard.state) {

                    case KeyboardState::Showing:
                        moveCursor(Direction::Down);
                        break;

                    default:
                        break;

                }

            }

            if (arduboy.justPressed(B_BUTTON)) {

                if (gamePlayVars.guesses.xCursor > 0) {

                    gamePlayVars.guesses.xCursor--;
                    gamePlayVars.guesses.chars[gamePlayVars.guesses.yCursor][gamePlayVars.guesses.xCursor] = ' ';
                    gamePlayVars.guesses.state[gamePlayVars.guesses.yCursor][gamePlayVars.guesses.xCursor] = GuessState::Dashed;

                }

            }

            if (arduboy.justPressed(A_BUTTON)) {

                if (gamePlayVars.keyboard.xCursor == 0 && gamePlayVars.keyboard.yCursor == 2) {

                    if (gamePlayVars.guesses.xCursor > 0) {

                        gamePlayVars.guesses.xCursor--;
                        gamePlayVars.guesses.chars[gamePlayVars.guesses.yCursor][gamePlayVars.guesses.xCursor] = ' ';
                        gamePlayVars.guesses.state[gamePlayVars.guesses.yCursor][gamePlayVars.guesses.xCursor] = GuessState::Dashed;

                    }

                }
                else if (gamePlayVars.keyboard.xCursor == 8 && gamePlayVars.keyboard.yCursor == 2) {

                    // Are there any spaces?

                    bool hasSpace = false;

                    for (uint8_t i = 0; i < 5; i++) {

                        if (gamePlayVars.guesses.chars[gamePlayVars.guesses.yCursor][i] == ' ') {

                            hasSpace = true;
                            break;

                        }

                    }

                    if (!hasSpace) {

                        gamePlayVars.checkState = checkWord();

                        switch (gamePlayVars.checkState) {

                            case CheckState::CorrectWord:
                                gamePlayVars.showInvalidWord_Count = 19;
                                break;

                            case CheckState::InvalidWord:
                                gamePlayVars.showInvalidWord_Count = 15;
                                break;

                            case CheckState::RealWord:

                                if (gamePlayVars.guesses.yCursor == 5) {

                                    gamePlayVars.checkState = CheckState::TooManyAttempts;

                                }
                                else {
                                    
                                    gamePlayVars.guesses.xCursor = 0;
                                    gamePlayVars.guesses.yCursor = gamePlayVars.guesses.yCursor + 1;

                                    if (gamePlayVars.guesses.yCursor >= 2) {
                                        gamePlayVars.guesses.listY = -((gamePlayVars.guesses.yCursor - 1) * Constants::guess_Spacing);
                                    }

                                }
                                break;

                            default: break;

                        }

                    }

                }
                else {
                    
                    //if (keyboard[Constants::key_Map[gamePlayVars.keyboard.yCursor][gamePlayVars.keyboard.xCursor]] != KeyState::Invisible) {

                        if (gamePlayVars.guesses.xCursor < 5) {

                            gamePlayVars.guesses.chars[gamePlayVars.guesses.yCursor][gamePlayVars.guesses.xCursor] = 65 + Constants::key_Map[gamePlayVars.keyboard.yCursor][gamePlayVars.keyboard.xCursor];

                        }

                        if (gamePlayVars.guesses.xCursor < 5) {

                            gamePlayVars.guesses.xCursor++;

                            if (gamePlayVars.guesses.xCursor == 5) {

                                gamePlayVars.keyboard.xCursor = 8;
                                gamePlayVars.keyboard.yCursor = 2;

                            }

                        }

                    //}

                }

            }

            break;

    }

    drawMan(tmpManX, tmpManB, tmpManI, tmpManP, tmpManK);
    drawGuesses(tmpManK, gamePlayVars.guesses.listY);

    switch (gamePlayVars.checkState) {

        case CheckState::InvalidWord:
            if (gamePlayVars.mode == GameMode::English) {
                FX::drawBitmap(23, 48, Images::InvalidWord_EN, 0, dbmNormal);
            }
            else {
                FX::drawBitmap(5, 48, Images::InvalidWord_FR, 0, dbmNormal);
            }
            break;

        case CheckState::CorrectWord:
            if (gamePlayVars.mode == GameMode::English) {
                FX::drawBitmap(41, 48, Images::Correct_EN, 0, dbmNormal);
            }
            else {
                FX::drawBitmap(41, 48, Images::Correct_FR, 0, dbmNormal);
            }
            break;

        case CheckState::TooManyAttempts:
            drawSolution();
            if (gamePlayVars.mode == GameMode::English) {
                FX::drawBitmap(18, 37, Images::TooManyAttempts_EN, 0, dbmNormal);
            }
            else {
                FX::drawBitmap(10, 37, Images::TooManyAttempts_FR, 0, dbmNormal);
            }
            break;

        case CheckState::Quit:
            arduboy.fillRect(0, 35, WIDTH, 32, BLACK);
            if (gamePlayVars.mode == GameMode::English) {
                FX::drawBitmap(29, 37, Images::Quit_EN, 0, dbmNormal);
            }
            else {
                FX::drawBitmap(32, 37, Images::Quit_FR, 0, dbmNormal);
            }
            drawSolution();
            break;

        default:
            drawKeyboard(19 + tmpManK, gamePlayVars.keyboard.yPos);
            break;

    }


    // Show or hide keyboard ..

    switch (gamePlayVars.keyboard.state) {

        case KeyboardState::StartHiding:

            gamePlayVars.keyboard.yPos++;
            if (gamePlayVars.guesses.yCursor >= 2) gamePlayVars.guesses.listY++;

            if (gamePlayVars.keyboard.yPos == 64) {
                gamePlayVars.keyboard.state = KeyboardState::Hiding;
            }
            break;

        case KeyboardState::StartShowing:

            gamePlayVars.keyboard.yPos--;
            if (gamePlayVars.guesses.yCursor >= 2) gamePlayVars.guesses.listY--;

            if (gamePlayVars.keyboard.yPos == 33) {
                gamePlayVars.keyboard.state = KeyboardState::Showing;
            }
            break;
        
        default:
            break;
    }



    // Housekeeping ..

    if (gamePlayVars.showInvalidWord_Count > 0 && arduboy.getFrameCount(2) == 0) gamePlayVars.showInvalidWord_Count--;

    if (arduboy.getFrameCount(3) == 0) {

        if (man_delay > 0) {
            man_delay--;

            if (man_delay == 0) {
                man_Idx = 0;
            }
        }
        else {

            switch (animation) {

                case AnimationSequence::Think_L:

                    man_Idx++;
                    if (man_Idx > 155) {
                        man_delay = random(Constants::Delay_Low, Constants::Delay_High);
                        animation = AnimationSequence::Walk_LR;
                    }

                    break;

                case AnimationSequence::Think_R:

                    man_Idx++;
                    if (man_Idx > 155) {
                        man_delay = random(Constants::Delay_Low, Constants::Delay_High);
                        animation = AnimationSequence::Walk_RL;
                    }

                    break;

                case AnimationSequence::Walk_LR:

                    man_Idx++;
                    if (man_Idx > 128 + 16) {
                        man_delay = random(Constants::Delay_Low, Constants::Delay_High);
                        animation = AnimationSequence::Think_R;
                    }

                    break;

                case AnimationSequence::Walk_RL:

                    man_Idx++;
                    if (man_Idx > 128 + 16) {
                        man_delay = random(Constants::Delay_Low, Constants::Delay_High);
                        animation = AnimationSequence::Think_L;
                    }

                    break;


            }

        }

    }

}


void drawSolution() {

    // Render words ..

    arduboy.setTextColor(BLACK);

    for (uint8_t x = 0; x < 5; x++) {

        arduboy.fillRect(Constants::guess_Left + (x * Constants::guess_Spacing), 50 - 2, Constants::guess_Spacing - 2, Constants::guess_Spacing - 2);
        arduboy.setCursor(Constants::guess_Left + (x * Constants::guess_Spacing) + 3, 50);
        arduboy.print(gamePlayVars.selectedWord[x]);

    }

    arduboy.setTextColor(WHITE);

}


void drawGuesses(int8_t xOffset, int8_t yOffset) {

    for (uint8_t y = 0; y < gamePlayVars.guesses.yCursor + 1; y++) {


        // Animate last row?

        int8_t xMove = 0;
        int8_t yMove[5] = { 0, 0, 0, 0, 0 };
        
        switch (gamePlayVars.checkState) {

            case CheckState::InvalidWord:

                if (y == gamePlayVars.guesses.yCursor) {
                    if (gamePlayVars.showInvalidWord_Count > 0) {
                        xMove = Constants::shake[gamePlayVars.showInvalidWord_Count];
                    }
                }
                break;

            case CheckState::CorrectWord:

                if (y == gamePlayVars.guesses.yCursor) {

                    if (gamePlayVars.showInvalidWord_Count > 0 && gamePlayVars.showInvalidWord_Count < 15) yMove[0] = Constants::shake[gamePlayVars.showInvalidWord_Count];
                    if (gamePlayVars.showInvalidWord_Count > 1 && gamePlayVars.showInvalidWord_Count < 16) yMove[1] = Constants::shake[gamePlayVars.showInvalidWord_Count - 1];
                    if (gamePlayVars.showInvalidWord_Count > 2 && gamePlayVars.showInvalidWord_Count < 17) yMove[2] = Constants::shake[gamePlayVars.showInvalidWord_Count - 2];
                    if (gamePlayVars.showInvalidWord_Count > 3 && gamePlayVars.showInvalidWord_Count < 18) yMove[3] = Constants::shake[gamePlayVars.showInvalidWord_Count - 3];
                    if (gamePlayVars.showInvalidWord_Count > 4 && gamePlayVars.showInvalidWord_Count < 19) yMove[4] = Constants::shake[gamePlayVars.showInvalidWord_Count - 4];

                }

                break;

            default: break;

        }


        // Render words ..

        for (uint8_t x = 0; x < 5; x++) {

            switch (gamePlayVars.guesses.state[y][x]) {

                case GuessState::Correct:
                    arduboy.setTextColor(BLACK);
                    arduboy.fillRect(Constants::guess_Left + (x * Constants::guess_Spacing) + xMove + xOffset, y * Constants::guess_Spacing + yOffset + yMove[x], Constants::guess_Spacing - 2, Constants::guess_Spacing - 2);
                    arduboy.setCursor(Constants::guess_Left + (x * Constants::guess_Spacing) + 3 + xMove + xOffset, y * Constants::guess_Spacing + 2 + yOffset + yMove[x]);
                    arduboy.print(gamePlayVars.guesses.chars[y][x]);
                    break;

                case GuessState::WrongPosition:
                    arduboy.setTextColor(WHITE);
                    arduboy.setCursor(Constants::guess_Left + (x * Constants::guess_Spacing) + 3 + xMove + xOffset, y * Constants::guess_Spacing + 2 + yOffset + yMove[x]);
                    arduboy.print(gamePlayVars.guesses.chars[y][x]);
                    arduboy.drawRect(Constants::guess_Left + (x * Constants::guess_Spacing) + xMove + xOffset, y * Constants::guess_Spacing + yOffset + yMove[x], Constants::guess_Spacing - 2, Constants::guess_Spacing - 2);
                    break;

                case GuessState::Dashed:
                    arduboy.setTextColor(WHITE);
                    arduboy.setCursor(Constants::guess_Left + (x * Constants::guess_Spacing) + 3 + xMove + xOffset, y * Constants::guess_Spacing + 2 + yOffset + yMove[x]);
                    arduboy.print(gamePlayVars.guesses.chars[y][x]);
                    arduboy.drawPixel(Constants::guess_Left + (x * Constants::guess_Spacing) + 1 + xMove + xOffset, ((y + 1) * Constants::guess_Spacing) - 2 + yOffset + yMove[x]);
                    arduboy.drawPixel(Constants::guess_Left + (x * Constants::guess_Spacing) + 3 + xMove + xOffset, ((y + 1) * Constants::guess_Spacing) - 2 + yOffset + yMove[x]);
                    arduboy.drawPixel(Constants::guess_Left + (x * Constants::guess_Spacing) + 5 + xMove + xOffset, ((y + 1) * Constants::guess_Spacing) - 2 + yOffset + yMove[x]);
                    arduboy.drawPixel(Constants::guess_Left + (x * Constants::guess_Spacing) + 7 + xMove + xOffset, ((y + 1) * Constants::guess_Spacing) - 2 + yOffset + yMove[x]);
                    arduboy.drawPixel(Constants::guess_Left + (x * Constants::guess_Spacing) + 9 + xMove + xOffset, ((y + 1) * Constants::guess_Spacing) - 2 + yOffset + yMove[x]);
                    break;


                default:
                    arduboy.setTextColor(WHITE);
                    arduboy.setCursor(Constants::guess_Left + (x * Constants::guess_Spacing) + 3 + xMove + xOffset, y * Constants::guess_Spacing + 2 + yOffset + yMove[x]);
                    arduboy.print(gamePlayVars.guesses.chars[y][x]);
                    break;

            }

        }

        arduboy.setTextColor(WHITE);

    }

}


void drawMan(uint8_t tmpManX, uint8_t tmpManB, uint8_t tmpManI, uint8_t tmpManP, int8_t tmpManK) {


    // Draw man?

    if (man_delay == 0) {

        switch (animation) {

            case AnimationSequence::Think_L:

                if (tmpManP > 0) {
                    FX::drawBitmap(2, 28, Images::Man_Word[tmpManP - 1], 0, dbmNormal);
                }

                switch (tmpManB) {

                    case 1:
                        FX::drawBitmap(tmpManX - 16, 36, Images::LightBulb, 0, dbmNormal);
                        break;

                }

                FX::drawBitmap(tmpManX - 16, 45, Images::Man[tmpManI], 0, dbmNormal); 
                break;

            case AnimationSequence::Think_R:
                
                if (tmpManP > 0) {
                    FX::drawBitmap(106, 28, Images::Man_Word[tmpManP - 1], 0, dbmNormal);
                }

                switch (tmpManB) {

                    case 1:
                        FX::drawBitmap(136 - tmpManX, 36, Images::LightBulb, 0, dbmNormal);
                        break;

                }

                FX::drawBitmap(136 - tmpManX, 45, Images::Man[tmpManI], 0, dbmNormal);
                break;
        
            case AnimationSequence::Walk_LR:
            case AnimationSequence::Walk_RL:
                FX::drawBitmap(tmpManX - 16, 45, Images::Man[tmpManI], 0, dbmNormal); 
                break;

        }

    }

}