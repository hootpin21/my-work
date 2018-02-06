#include "ConnectGameState.h"
#include "MenuGameState.h"

namespace platformer {

    ConnectGameState::ConnectGameState(Game *game) : game(game) {}

    void ConnectGameState::onButtonAPress() {
        stop();
    }

    void ConnectGameState::onButtonBPress() {
        stop();
    }

    void ConnectGameState::onButtonABPress() {
        stop();
    }

    void ConnectGameState::onMessage(ByteBuf &) {
        // No packets should be received in this state.
    }

    void ConnectGameState::run() {
        game->setMultiplayer(true);
        game->getMicroBit()->radio.enable();

        while (game->getState() == this) {
            // Create and send the broadcast packet.
            ByteBuf out;
            out.writeInt(GAME_ID);
            out.writeInt(game->getId());
            out.writeInt(0);
            out.writePacketType(PacketType::BROADCAST);

            game->sendPacket(out);

            // Display "CONNECTING" animation.
            game->getMicroBit()->display.scrollAsync("CONNECTING");

            // Sleep until next tick should occur.
            game->getMicroBit()->sleep(TICK_RATE);
        }

        // Disable "CONNECTING" scrolling animation.
        game->getMicroBit()->display.stopAnimation();

        MicroBitImage *face = game->getScreen();
        face->clear();

        if (game->isConnected()) {
            // Create a happy face.
            face->setPixelValue(1, 1, 255);
            face->setPixelValue(3, 1, 255);
            face->setPixelValue(0, 3, 255);
            face->setPixelValue(1, 4, 255);
            face->setPixelValue(2, 4, 255);
            face->setPixelValue(3, 4, 255);
            face->setPixelValue(4, 3, 255);
        } else {
            // Create a sad face.
            face->setPixelValue(1, 1, 255);
            face->setPixelValue(3, 1, 255);
            face->setPixelValue(0, 4, 255);
            face->setPixelValue(1, 3, 255);
            face->setPixelValue(2, 3, 255);
            face->setPixelValue(3, 3, 255);
            face->setPixelValue(4, 4, 255);
        }

        // Paste face to the screen.
        game->getMicroBit()->display.image.paste(*face);
        game->getMicroBit()->sleep(TICK_RATE);
        delete this;
    }

    void ConnectGameState::stop() {
        // Disable multiplayer.
        game->disconnect();

        // Switch to the menu game state.
        auto *nextState = new MenuGameState(game);
        game->setState(nextState);
    };

}
