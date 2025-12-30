#pragma once

#include "creature.hpp"

class Herbivore : public Creature {
   public:
    Herbivore(int x, int y, MoveDirection dir, int interval);

    void updatePosition(int mapWidth, int mapHeight) override;
    bool readyToBreed() const override;
    std::unique_ptr<Creature> createOffspring() const override;
    CreatureKind kind() const override;
    int lifespan() const override;
    int velocity() const override;
    std::string symbol() const override;

    void onReproduced() noexcept;

   private:
    static constexpr int MAX_LIFESPAN = 15;
    static constexpr int FIRST_BREED_AGE = 5;
    static constexpr int SECOND_BREED_AGE = 10;
    static constexpr int MOVEMENT_SPEED = 1;

    int timesReproduced_{0};
};