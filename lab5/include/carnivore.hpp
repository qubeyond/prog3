#pragma once

#include "creature.hpp"

class Carnivore : public Creature {
   public:
    Carnivore(int x, int y, MoveDirection dir, int interval);

    void updatePosition(int mapWidth, int mapHeight) override;
    bool readyToBreed() const override;
    std::unique_ptr<Creature> createOffspring() const override;
    CreatureKind kind() const override;
    int lifespan() const override;
    int velocity() const override;
    std::string symbol() const override;

    void consumePrey();
    void resetHunger();
    int getMealsConsumed() const noexcept {
        return mealsConsumed_;
    }

   private:
    static constexpr int MAX_LIFESPAN = 25;
    static constexpr int MEALS_FOR_BREEDING = 2;
    static constexpr int MOVEMENT_SPEED = 2;

    int mealsConsumed_{0};
};