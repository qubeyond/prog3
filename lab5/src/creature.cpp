#include "creature.hpp"

Creature::Creature(int x, int y, MoveDirection dir, int interval)
    : posX_(x), posY_(y), heading_(dir), turnInterval_(interval) {
}

void Creature::growOlder() noexcept {
    ++currentAge_;
}

bool Creature::hasExpired() const noexcept {
    return currentAge_ >= lifespan();
}

void Creature::turnClockwise() noexcept {
    heading_ = static_cast<MoveDirection>((static_cast<int>(heading_) + 1) % 4);
}

void Creature::moveTo(int newX, int newY) noexcept {
    posX_ = newX;
    posY_ = newY;
}