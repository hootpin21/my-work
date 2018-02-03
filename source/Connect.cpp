#include "Connect.h"
#include "Menu.h"

namespace platformer {

    Connect::Connect(Game *game) : game(game) {}

    void Connect::onButtonAPress() {
        stop();
    }

    void Connect::onButtonBPress() {
        stop();
    }

    void Connect::onButtonABPress() {
        stop();
    }

    void Connect::onMessage(ByteBuf &in) {
    }

    void Connect::run() {
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
            face->setPixelValue(0, 4, 255);
            face->setPixelValue(1, 3, 255);
            face->setPixelValue(2, 3, 255);
            face->setPixelValue(3, 3, 255);
            face->setPixelValue(4, 4, 255);
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

    void Connect::stop() {
        // Disable multiplayer.
        game->setMultiplayer(false);
        game->setConnected(false);
        game->setPartnerId(0);

        // Switch to the menu game state.
        auto *nextState = new Menu(game);
        game->setState(nextState);
    };
}
