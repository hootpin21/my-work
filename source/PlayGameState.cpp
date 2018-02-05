#include "PlayGameState.h"
#include "DeathGameState.h"
#include "MenuGameState.h"

namespace platformer {

    PlayGameState::PlayGameState(Game *game, World *world) : game(game), world(world) {}

    PlayGameState::~PlayGameState() {
        delete player;
        delete world;
    }

    void PlayGameState::onButtonAPress() {
        partnerComplete = true;

        // Send quit world packet when in multiplayer.
        if (game->isMultiplayer()) {
            ByteBuf out = game->createPacket();
            out.writePacketType(PacketType::QUIT_WORLD);
            game->sendPacket(out);
        }

        // Go back to the main menu.
        quitToMenu();
    }

    void PlayGameState::onButtonBPress() {
        jump();
    }

    void PlayGameState::onButtonABPress() {
        partnerComplete = true;
        game->disconnect();
        quitToMenu();
    }

    void PlayGameState::onMessage(ByteBuf &in) {
        PacketType packetType = in.readPacketType();

        switch (packetType) {
            case PacketType::WORLD_COMPLETE: {
                if (complete) {
                    game->getMicroBit()->display.stopAnimation();
                }
                partnerScore = in.readInt();
                partnerComplete = true;
                return;
            }
            case PacketType::QUIT_WORLD: {
                partnerComplete = true;
                quitToMenu();
                return;
            }
            case PacketType::DISCONNECT: {
                partnerComplete = true;
                game->disconnect();
                quitToMenu();
                return;
            }
            default: {
                return;
            }
        }
    }

    void PlayGameState::quitToMenu() const {
        game->getMicroBit()->display.stopAnimation();
        auto *nextState = new MenuGameState(game);
        game->setState(nextState);
    }

    void PlayGameState::jump() {
        BlockType below = world->getBlock(player->getLocation().clone().addY(-1));
        if (below == SOLID) {
            player->jump();
        }
    }

    void PlayGameState::run() {
        score = 0;
        player->getLocation().set(1, 1);

        while (game->getState() == this) {
            game->getScreen()->clear();
            tick();
            render();
            game->getMicroBit()->display.image.paste(*game->getScreen());
            game->getMicroBit()->sleep(TICK_RATE);
        }

        delete this;
    }

    void PlayGameState::tick() {
        Vector2i &location = player->getLocation();
        Vector2i &velocity = player->getVelocity();

        BlockType center = world->getBlock(location);
        BlockType above = world->getBlock(location.getRelative(0, 1));
        BlockType below = world->getBlock(location.getRelative(0, -1));

        if (center == FLAG) {
            handleCompletion();
            return;
        }

        if (center == COIN) {
            world->setBlock(location, AIR);
            score++;
        }

        if (above == SOLID && velocity.getY() > 0) {
            velocity.setY(0);
        }

        if (below != SOLID) {
            velocity.addY(-1);
        }

        if (velocity.getY() > 0) {
            location.addY(1);
        } else if (velocity.getY() < 0) {
            if (below == SOLID) {
                velocity.setY(0);
            } else {
                location.addY(-1);

                if (location.getY() < 0) {
                    handleDeath();
                    return;
                }
            }
        }

        BlockType left = world->getBlock(location.getRelative(-1, 0));
        BlockType right = world->getBlock(location.getRelative(1, 0));
        int accelerometerX = game->getMicroBit()->accelerometer.getX();

        if (accelerometerX < -300 && location.getX() > 0 && left != SOLID) {
            location.addX(-1);
        }

        if (accelerometerX > 300 && location.getX() < (world->getMaxX() - 2) && right != SOLID) {
            location.addX(1);
        }

        if (velocity.getX() > 0) {
            location.addX(1);
        } else if (velocity.getX() < 0) {
            location.addX(-1);
        }

        displayCoins = !displayCoins;
    }

    void PlayGameState::handleCompletion() {
        complete = true;

        if (game->isMultiplayer()) {
            // Message the partner that we have complete the level.
            ByteBuf out = game->createPacket();
            out.writePacketType(PacketType::WORLD_COMPLETE);
            out.writeInt(score);
            game->sendPacket(out);

            // Wait until either the partner dies or completes the level.
            while (!partnerComplete) {
                game->getMicroBit()->display.scroll("WAITING", 80);
                game->getMicroBit()->sleep(TICK_RATE);
            }

            // Display the status message and score.
            if (partnerScore < score) {
                game->getMicroBit()->display.scroll("WINNER! SCORE:", 80);
            } else if (partnerScore > score) {
                game->getMicroBit()->display.scroll("LOOSER! SCORE:", 80);
            } else {
                game->getMicroBit()->display.scroll("DRAW! SCORE:", 80);
            }
        } else {
            game->getMicroBit()->display.scroll("WINNER! SCORE:", 80);
        }

        game->getMicroBit()->display.scroll(score, 80);
        quitToMenu();
    }

    void PlayGameState::handleDeath() const {
        if (!partnerComplete) {
            // Temporarily switch to game over screen.
            auto *nextState = new DeathGameState(game, world->getId());
            game->setState(nextState);
            return;
        }

        // Create and send world complete packet, as died after partner is complete.
        ByteBuf out = game->createPacket();
        out.writePacketType(PacketType::WORLD_COMPLETE);
        out.writeInt(-1); // Player did not complete course, thus is given a negative score.
        game->sendPacket(out);

        // Show that we have lost the game.
        game->getMicroBit()->display.scroll("LOOSER! SCORE: 0", 80);

        // Update next state to the main menu.
        quitToMenu();
    }

    void PlayGameState::render() const {
        // Render the player.
        Vector2i location = player->getLocation();
        int offsetX = SCREEN_CENTER;
        int offsetY = SCREEN_CENTER;

        if (location.getY() <= SCREEN_CENTER) {
            offsetY -= SCREEN_CENTER - location.getY();
        }

        if (location.getY() >= (world->getMaxY() - SCREEN_CENTER)) {
            offsetY += SCREEN_CENTER - ((world->getMaxY() - 1) - location.getY());
        }

        if (location.getX() <= SCREEN_CENTER) {
            offsetX -= SCREEN_CENTER - location.getX();
        }

        if (location.getX() >= (world->getMaxX() - SCREEN_CENTER - 1)) {
            offsetX += SCREEN_CENTER - ((world->getMaxX() - 2) - location.getX());
        }

        game->getScreen()->setPixelValue((uint16_t) offsetX, (uint16_t) (4 - offsetY), 255);

        // Render the map.
        for (int x = 0; x < SCREEN_SIZE; x++) {
            for (int y = 0; y < SCREEN_SIZE; y++) {
                renderBlock(offsetX, offsetY, x, y);
            }
        }
    }

    void PlayGameState::renderBlock(int offsetX, int offsetY, int x, int y) const {
        BlockType blockType = world->getBlock(player->getLocation().getRelative(x - offsetX, y - offsetY));

        switch (blockType) {
            case AIR:
                break;
            case SOLID:
                game->getScreen()->setPixelValue((uint16_t) x, (uint16_t) (4 - y), 16);
                break;
            case FLAG:
                game->getScreen()->setPixelValue((uint16_t) x, (uint16_t) (4 - y), 48);
                break;
            case COIN:
                if (displayCoins) {
                    game->getScreen()->setPixelValue((uint16_t) x, (uint16_t) (4 - y), 96);
                }
                break;
        }
    }

}
