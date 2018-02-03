#include "Menu.h"
#include "Session.h"
#include "Connect.h"

namespace platformer {

    Menu::Menu(Game *game) : game(game) {}

    void Menu::onButtonAPress() {
        game->getMicroBit()->display.stopAnimation();
    }

    void Menu::onButtonBPress() {
        // Stop all current screen animations.
        game->getMicroBit()->display.stopAnimation();

        // Update the next state to a game session in the currently selected world.
        auto *nextState = new Session(game, createWorld(selectedWorld));
        game->setState(nextState);

        // If in multiplayer, tell our partner to join the selected world.
        if (game->isMultiplayer()) {
            ByteBuf out = game->createPacket();
            out.writePacketType(PacketType::SELECT_WORLD);
            out.writeInt(selectedWorld);
            game->sendPacket(out);
        }
    }

    void Menu::onButtonABPress() {
        // Toggle whether the game is in multiplayer mode.
        game->setMultiplayer(!game->isMultiplayer());

        if (game->isMultiplayer()) {

            // Switch to the connect game state.
            auto *nextState = new Connect(game);
            game->setState(nextState);

        } else if (game->isConnected()) {

            // Disconnect from partner.
            ByteBuf out = game->createPacket();
            out.writeInt(DISCONNECT);
            game->sendPacket(out);
            game->setConnected(false);
            game->setPartnerId(0);

            // Let the user now they've successfully disconnected.
            game->getMicroBit()->display.scroll("DISCONNECTED", 80);

        }
    }

    void Menu::onMessage(ByteBuf &in) {
        PacketType packetType = in.readPacketType();

        switch (packetType) {
            case SELECT_WORLD: {
                selectedWorld = in.readInt();
                game->getMicroBit()->display.stopAnimation();

                auto *nextState = new Session(game, createWorld(selectedWorld));
                game->setState(nextState);
            }
            default: {
                return;
            }
        }
    }

    void Menu::run() {
        game->getMicroBit()->display.scroll("SELECT WORLD", 80);

        while (game->getState() == this) {
            game->getScreen()->clear();
            tick();
            render();
            game->getMicroBit()->sleep(TICK_RATE);
        }

        delete this;
    }

    void Menu::tick() {
        int accelerometerX = game->getMicroBit()->accelerometer.getX();

        if (accelerometerX < -300) {
            selectedWorld--;

            if (selectedWorld <= 0) {
                selectedWorld = WORLD_COUNT;
            }
        }

        if (accelerometerX > 300) {
            selectedWorld++;

            if (selectedWorld > WORLD_COUNT) {
                selectedWorld = 1;
            }
        }
    }

    void Menu::render() const {
        game->getMicroBit()->display.print(selectedWorld);
    }

}
