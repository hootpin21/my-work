#ifndef PLATFORMER_GAME_H
#define PLATFORMER_GAME_H

#include "MicroBit.h"
#include "GameState.h"

namespace platformer {

    // The ID of this game, used for filtering nonsense packets.
    static const int GAME_ID = 0x1337;

    // Size of the micro:bit LED display.
    // The LED count for both X and Y are the same.
    static const int SCREEN_SIZE = 5;

    // Global for all game text scroll speed.
    static const int SCROLL_SPEED = 80;

    /**
     * The main game manager. Holds and manages the current game state and
     * provides quick access to device information.
     */
    class Game {
    private:
        // The device serial number, used to uniquely identify the device while
        // communicating over the network.
        const uint32_t id = microbit_serial_number();

        // The micro:bit instance, used for interacting with the hardware.
        MicroBit *microBit = new MicroBit();

        // Temporary screen to be cleared and written to, then pasting to the
        // device LEDs. When used, this should provide quick rendering with no
        // image stuttering issues.
        MicroBitImage *screen = new MicroBitImage(SCREEN_SIZE, SCREEN_SIZE);

        // The current game state.
        GameState *state = nullptr;

        // Determines whether the game is currently changing state. Used for
        // not routing events to non-running states.
        bool changingState = false;

        // Multiplayer settings.
        bool multiplayer = false;
        bool connected = false;
        uint32_t partnerId = 0;

        /**
         * Handles when button A is pressed.
         */
        void onButtonAPress(MicroBitEvent);

        /**
         * Handles when button B is pressed.
         */
        void onButtonBPress(MicroBitEvent);

        /**
         * Handles when both buttons A and B are pressed together.
         */
        void onButtonABPress(MicroBitEvent);

        /**
         * Handles when a radio packet has been received.
         */
        void onMessage(MicroBitEvent);

        /**
         * Attempts to create a connection, given the following connection
         * initiation arguments. Should only be called when not already
         * connected to another micro:bit, and we are being requested to do so.
         *
         * @param packetType the packet type we were sent.
         * @param senderId the sender ID.
         * @param targetId the target ID.
         */
        void setupConnection(PacketType packetType, uint32_t senderId, uint32_t targetId);

        /**
         * Sets whether we are connected to another device for multiplayer mode.
         *
         * @param connected {@code true} if we are connected.
         */
        void setConnected(bool connected);

        /**
         * Sets the currently connected partner ID.
         *
         * @param partnerId the partner ID.
         */
        void setPartnerId(uint32_t partnerId);

    public:
        /**
         * Constructs a new {@link Game}.
         */
        Game();

        /**
         * Destructs the current {@link Game}, deletes the
         */
        virtual ~Game();

        /**
         * Runs the game.
         */
        void run();

        /**
         * Creates a new packet, writing the {@link GAME_ID}, sender ID and
         * the target ID.
         *
         * @return the newly created packet.
         */
        ByteBuf createPacket();

        /**
         * Sends a written packet over the micro:bit radio.
         *
         * @param buf the {@link ByteBuf} packet.
         */
        void sendPacket(ByteBuf buf);

        /**
         * Disconnects from a connected and disables micro:bit radio.
         */
        void disconnect();

        /**
         * Gets the {@link MicroBit} instance.
         *
         * @return the micro:bit API (or uBit, in micro:bit-samples).
         */
        MicroBit *getMicroBit() const;

        /**
         * Gets the temporary screen cache for safe LED writes.
         *
         * @return the micro:bit image.
         */
        MicroBitImage *getScreen() const;

        /**
         * Gets the current game state.
         *
         * @return the current game state.
         */
        GameState *getState() const;

        /**
         * Sets the current game state. Note: This does not immediately switch
         * the state, the state currently being ran must finish execution and
         * cleanup beforehand.
         *
         * @param state the new state to switch to.
         */
        void setState(GameState *state);

        /**
         * Gets whether the game is currently switching state.
         *
         * @return {@code true} if currently switching state.
         */
        bool isChangingState() const;

        /**
         * Gets whether the game is using multiplayer.
         *
         * @return {@code true} if using multiplayer.
         */
        bool isMultiplayer() const;

        /**
         * Sets whether the game should be using multiplayer.
         *
         * @param multiplayer {@code true} if using multiplayer.
         */
        void setMultiplayer(bool multiplayer);

        /**
         * Gets whether we are currently connected to another device for
         * multiplayer mode.
         *
         * @return {@code true} if we are connected.
         */
        bool isConnected() const;

        /**
         * Gets the unique identifier for our device, useful for communicating
         * via the micro:bit radio service.
         *
         * @return this devices unique identifier.
         */
        uint32_t getId() const;

        /**
         * Gets the currently connect partner devices unique identifier.
         *
         * @return the partners ID.
         */
        uint32_t getPartnerId() const;
    };

}

#endif // PLATFORMER_GAME_H
