#include "carnivore.hpp"

Carnivore::Carnivore(int x, int y, MoveDirection dir, int interval)
    : Creature(x, y, dir, interval) {
}

void Carnivore::updatePosition(int mapWidth, int mapHeight) {
    int dx = 0, dy = 0;

    switch (heading_) {
        case MoveDirection::NORTH:
            dy = -MOVEMENT_SPEED;
            break;
        case MoveDirection::EAST:
            dx = MOVEMENT_SPEED;
            break;
        case MoveDirection::SOUTH:
            dy = MOVEMENT_SPEED;
            break;
        case MoveDirection::WEST:
            dx = -MOVEMENT_SPEED;
            break;
    }

    int nextX = ((posX_ + dx) % mapWidth + mapWidth) % mapWidth;
    int nextY = ((posY_ + dy) % mapHeight + mapHeight) % mapHeight;
    moveTo(nextX, nextY);

    if (++moveCount_ >= turnInterval_) {
        turnClockwise();
        moveCount_ = 0;
    }
}

bool Carnivore::readyToBreed() const {
    return mealsConsumed_ >= MEALS_FOR_BREEDING;
}

std::unique_ptr<Creature> Carnivore::createOffspring() const {
    return std::make_unique<Carnivore>(posX_, posY_, heading_, turnInterval_);
}

CreatureKind Carnivore::kind() const {
    return CreatureKind::CARNIVORE;
}

int Carnivore::lifespan() const {
    return MAX_LIFESPAN;
}

int Carnivore::velocity() const {
    return MOVEMENT_SPEED;
}

std::string Carnivore::symbol() const {
    return "W";
}

void Carnivore::consumePrey() {
    ++mealsConsumed_;
}

void Carnivore::resetHunger() {
    mealsConsumed_ = 0;
}