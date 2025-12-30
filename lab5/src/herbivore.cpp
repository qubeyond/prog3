#include "herbivore.hpp"

Herbivore::Herbivore(int x, int y, MoveDirection dir, int interval)
    : Creature(x, y, dir, interval) {
}

void Herbivore::updatePosition(int mapWidth, int mapHeight) {
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

bool Herbivore::readyToBreed() const {
    bool firstTime = (currentAge_ == FIRST_BREED_AGE) && (timesReproduced_ == 0);
    bool secondTime = (currentAge_ == SECOND_BREED_AGE) && (timesReproduced_ == 1);
    return firstTime || secondTime;
}

std::unique_ptr<Creature> Herbivore::createOffspring() const {
    return std::make_unique<Herbivore>(posX_, posY_, heading_, turnInterval_);
}

CreatureKind Herbivore::kind() const {
    return CreatureKind::HERBIVORE;
}

int Herbivore::lifespan() const {
    return MAX_LIFESPAN;
}

int Herbivore::velocity() const {
    return MOVEMENT_SPEED;
}

std::string Herbivore::symbol() const {
    return "R";
}

void Herbivore::onReproduced() noexcept {
    ++timesReproduced_;
}