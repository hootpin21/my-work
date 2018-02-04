#include "DeathGameState.h"
#include "PlayGameState.h"
#include "MenuGameState.h"

namespace platformer {

    DeathGameState::DeathGameState(Game *game, int worldId) : game(game), worldId(worldId) {}

    void DeathGameState::onButtonAPress() {
        // Do nothing if currently in multiplayer.
        if (game->isMultiplayer()) {
            return;
        }

        // Go back to the main menu.
        game->getMicroBit()->display.stopAnimation();
        auto *nextState = new MenuGameState(game);
        game->setState(nextState);
    }

    void DeathGameState::onButtonBPress() {
    }

    void DeathGameState::onButtonABPress() {
    }

    void DeathGameState::onMessage(ByteBuf &in) {
        PacketType packetType = in.readPacketType();

        switch (packetType) {
            case PacketType::WORLD_COMPLETE: {
                // Create and send world complete packet, as died after partner is complete.
                ByteBuf out = game->createPacket();
                out.writePacketType(PacketType::WORLD_COMPLETE);
                out.writeInt(-1); // Player did not complete course, thus is given a negative score.
                game->sendPacket(out);

                // Show that we have lost the game.
                game->getMicroBit()->display.scroll("LOOSER!", 80);

                // Go back to the main menu.
                auto *nextState = new MenuGameState(game);
                game->setState(nextState);
                return;
            }
            default: {
                return;
            }
        }
    }

    void DeathGameState::run() {
        counter = 0;

        while (game->getState() == this) {
            game->getScreen()->clear();
            tick();
            render();
            game->getMicroBit()->display.image.paste(*game->getScreen());
            game->getMicroBit()->sleep(TICK_RATE);
        }

        delete this;
    }

    void DeathGameState::tick() {
        counter++;

        if (counter >= FLASH_COUNT) {
            auto *nextState = new PlayGameState(game, createWorld(worldId));
            game->setState(nextState);
        }
    }

    void DeathGameState::render() const {
        if (counter % 2 == 0) {
            return;
        }

        for (uint16_t x = 0; x < SCREEN_SIZE; x++) {
            for (uint16_t y = 0; y < SCREEN_SIZE; y++) {
                game->getScreen()->setPixelValue(x, y, 255);
            }
        }
    }

}
