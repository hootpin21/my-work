#include "Game.h"
#include "MenuGameState.h"

namespace platformer {

    namespace {
        Game *game = nullptr; // Singleton reference.

        void onButtonAPress(MicroBitEvent) {
            if (!game->isChangingState()) {
                game->getState()->onButtonAPress();
            }
        }

        void onButtonBPress(MicroBitEvent) {
            if (!game->isChangingState()) {
                game->getState()->onButtonBPress();
            }
        }

        void onButtonABPress(MicroBitEvent) {
            if (!game->isChangingState()) {
                game->getState()->onButtonABPress();
            }
        }

        void onMessage(MicroBitEvent) {
            // Create byte buffer for easy message reading.
            PacketBuffer received = game->getMicroBit()->radio.datagram.recv();
            ByteBuf in;
            for (int i = 0; i < received.length(); i++) {
                in.write(received.getByte(i));
            }

            // Read game ID, if message is not for this game then ignore.
            int gameId = in.readInt();
            if (gameId != GAME_ID) {
                return;
            }

            // Read the sender and target IDs.
            int senderId = in.readInt();
            int targetId = in.readInt();

            if (!game->isConnected()) {
                PacketType packetType = in.readPacketType();

                // Neither sender nor recipient IDs should be negative.
                if ((senderId | targetId) < 0) {
                    return;
                }

                // Begin writing response.
                ByteBuf out;
                out.writeInt(GAME_ID);

                switch (packetType) {
                    case PacketType::BROADCAST: {
                        // Broadcasts must have a target ID of 0.
                        if (targetId != 0) {
                            return;
                        }

                        // Write partner request.
                        out.writeInt(game->getId()); // Sender ID.
                        out.writeInt(senderId);      // Target ID.
                        out.writePacketType(PacketType::PARTNER_REQUEST);
                        break;
                    }

                    case PacketType::PARTNER_REQUEST: {
                        // Ignore partner requests of invalid ID.
                        if (targetId != game->getId()) {
                            return;
                        }

                        // Write another partner request to accept.
                        out.writeInt(game->getId()); // Sender ID.
                        out.writeInt(senderId);      // Target ID.
                        out.writePacketType(PacketType::PARTNER_REQUEST);

                        // Update connection info.
                        game->setConnected(true);
                        game->setPartnerId(senderId);

                        // Switch to the menu game state.
                        auto *nextState = new MenuGameState(game);
                        game->setState(nextState);
                        break;
                    }

                    default: {
                        // Ignore invalid packets for this state.
                        return;
                    }
                }

                // On successful interpretation and a response is written, send it.
                game->sendPacket(out);
            }

            // Ignore messages from non-partnered devices.
            if (senderId != game->getPartnerId()) {
                return;
            }

            // Disconnect if partner is found communicating with others.
            if (targetId != game->getId()) {
                game->disconnect();
                return;
            }

            // Wait until state has changed.
            while (game->isChangingState()) {
                game->getMicroBit()->sleep(1);
            }

            // Inform the current running state of this message.
            game->getState()->onMessage(in);
        }
    }

    Game::Game() {
        game = this;
    }

    Game::~Game() {
        delete microBit;
        delete screen;
        delete state;
    }

    void Game::run() {
        // Register event handlers.
        microBit->messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonAPress);
        microBit->messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonBPress);
        microBit->messageBus.listen(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK, onButtonABPress);
        microBit->messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onMessage);

        // State is now the main menu.
        state = new MenuGameState(this);

        // Now enter the game loop.
        while (state != nullptr) {
            changingState = false;
            state->run();
        }
    }

    ByteBuf Game::createPacket() {
        ByteBuf buf;
        buf.writeInt(GAME_ID);      // Game ID
        buf.writeInt(id);           // Sender ID
        buf.writeInt(partnerId);    // Target ID
        return buf;
    }

    void Game::disconnect() {
        if (!game->isMultiplayer()) {
            return;
        }

        // Send disconnect packet.
        ByteBuf out = createPacket();
        out.writePacketType(PacketType::DISCONNECT);
        sendPacket(out);

        // Update game state, disabling connection and the radio.
        game->getMicroBit()->radio.disable();
        game->setMultiplayer(false);
        game->setConnected(false);
        game->setPartnerId(0);
    }

    void Game::sendPacket(ByteBuf packet) {
        microBit->radio.datagram.send(packet.data(), packet.size());
    }

    MicroBit *Game::getMicroBit() const {
        return microBit;
    }

    MicroBitImage *Game::getScreen() const {
        return screen;
    }

    GameState *Game::getState() const {
        return state;
    }

    void Game::setState(GameState *state) {
        changingState = true;
        Game::state = state;
    }

    bool Game::isChangingState() const {
        return changingState;
    }

    bool Game::isMultiplayer() const {
        return multiplayer;
    }

    void Game::setMultiplayer(bool multiplayer) {
        Game::multiplayer = multiplayer;
    }

    bool Game::isConnected() const {
        return connected;
    }

    void Game::setConnected(bool connected) {
        Game::connected = connected;
    }

    int Game::getId() const {
        return id;
    }

    int Game::getPartnerId() const {
        return partnerId;
    }

    void Game::setPartnerId(int partnerId) {
        Game::partnerId = partnerId;
    }

}
