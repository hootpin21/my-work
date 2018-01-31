#ifndef PLATFORMER_GAMESTATE_H
#define PLATFORMER_GAMESTATE_H

namespace platformer {

    class GameState {
    public:
        virtual ~GameState() = default;
        virtual void onButtonAPress() = 0;
        virtual void onButtonBPress() = 0;
        virtual void run() = 0;
    };

}

#endif //PLATFORMER_GAMESTATE_H
