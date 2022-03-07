#include <Arduboy2.h>

#pragma once

#define SOUNDS
#define SPLASH
#define _THREE_IMAGES
#define SHOW_INSTUCTIONS
#define SHOW_ROAR
#define RESET

struct TitleScreenVars {

//    GameMode cursor = 0;

    void reset() {

        // this->cursor = GameMode::English;

    }

};

struct GamePlayVars {

   GameMode mode = GameMode::English;

    void reset() {

        this->mode = GameMode::English;

    }

};
