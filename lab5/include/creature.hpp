#pragma once

#include <memory>
#include <string>

enum class MoveDirection { NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3 };

enum class CreatureKind { HERBIVORE, CARNIVORE };

class Creature {
   public:
    Creature(int x, int y, MoveDirection dir, int interval);
    virtual ~Creature() = default;

    virtual void updatePosition(int mapWidth, int mapHeight) = 0;
    virtual bool readyToBreed() const = 0;
    virtual std::unique_ptr<Creature> createOffspring() const = 0;
    virtual CreatureKind kind() const = 0;
    virtual int lifespan() const = 0;
    virtual int velocity() const = 0;
    virtual std::string symbol() const = 0;

    void growOlder() noexcept;
    bool hasExpired() const noexcept;
    void turnClockwise() noexcept;
    void moveTo(int newX, int newY) noexcept;

    int x() const noexcept {
        return posX_;
    }
    int y() const noexcept {
        return posY_;
    }
    int age() const noexcept {
        return currentAge_;
    }
    MoveDirection direction() const noexcept {
        return heading_;
    }
    int interval() const noexcept {
        return turnInterval_;
    }

   protected:
    int posX_, posY_;
    MoveDirection heading_;
    int turnInterval_;
    int currentAge_{0};
    int moveCount_{0};
};