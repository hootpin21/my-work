#include "Session.h"
#include "GameOver.h"
#include "Menu.h"

namespace platformer {

    Session::Session(Game *game, World *world) : game(game), world(world) {}

    Session::~Session() {
        delete player;
        delete world;
    }

    void Session::onButtonAPress() {
        // Do nothing if currently in multiplayer.
        if (game->isMultiplayer()) {
            return;
        }

        // Go back to the main menu.
        game->getMicroBit()->display.stopAnimation();
        auto *nextState = new Menu(game);
        game->setState(nextState);
    }

    void Session::onButtonBPress() {
        jump();
    }

    void Session::onButtonABPress() {
    }

    void Session::onMessage(ByteBuf &in) {
        PacketType packetType = in.readPacketType();

        switch (packetType) {
            case PacketType::WORLD_COMPLETE: {
                partnerScore = in.readInt();
                return;
            }
            default: {
                return;
            }
        }
    }

    void Session::jump() {
        BlockType below = world->getBlock(player->getLocation().clone().addY(-1));
        if (below == SOLID) {
            player->jump();
        }
    }

    void Session::run() {
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

    void Session::tick() {
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

    void Session::handleCompletion() const {
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

        // Go back to the main menu.
        auto *nextState = new Menu(game);
        game->setState(nextState);
    }

    void Session::handleDeath() const {
        if (!partnerComplete) {
            // Temporarily switch to game over screen.
            auto *nextState = new GameOver(game, world->getId());
            game->setState(nextState);
            return;
        }

        // Create and send world complete packet, as died after partner is complete.
        ByteBuf out = game->createPacket();
        out.writePacketType(PacketType::WORLD_COMPLETE);
        out.writeInt(-1); // Player did not complete course, thus is given a negative score.
        game->sendPacket(out);

        // Show that we have lost the game.
        game->getMicroBit()->display.scroll("LOOSER!", 80);

        // Go back to the main menu.
        auto *nextState = new Menu(game);
        game->setState(nextState);
    }

    void Session::render() const {
        // Render the player.
        Vector2i location = player->getLocation();
        int offsetX = HALF_SCREEN;
        int offsetY = HALF_SCREEN;

        if (location.getY() <= HALF_SCREEN) {
            offsetY -= HALF_SCREEN - location.getY();
        }

        if (location.getY() >= (world->getMaxY() - HALF_SCREEN)) {
            offsetY += HALF_SCREEN - ((world->getMaxY() - 1) - location.getY());
        }

        if (location.getX() <= HALF_SCREEN) {
            offsetX -= HALF_SCREEN - location.getX();
        }

        if (location.getX() >= (world->getMaxX() - HALF_SCREEN - 1)) {
            offsetX += HALF_SCREEN - ((world->getMaxX() - 2) - location.getX());
        }

        game->getScreen()->setPixelValue((uint16_t) offsetX, (uint16_t) (4 - offsetY), 255);

        // Render the map.
        for (int x = 0; x < SCREEN_SIZE; x++) {
            for (int y = 0; y < SCREEN_SIZE; y++) {
                renderBlock(offsetX, offsetY, x, y);
            }
        }
    }

    void Session::renderBlock(int offsetX, int offsetY, int x, int y) const {
        BlockType blockType = world->getBlock(player->getLocation().clone().add(x - offsetX, y - offsetY));

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
