#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "creature.hpp"

class Ecosystem {
   public:
    Ecosystem(int w, int h, int steps);

    void introduceCreature(std::unique_ptr<Creature> c);
    void simulate(bool showProgress = false);
    void displayResult() const;
    void displayState(int stepNum) const;

    size_t getPopulationSize() const noexcept {
        return population_.size();
    }

   private:
    int fieldWidth_;
    int fieldHeight_;
    int totalSteps_;
    std::vector<std::unique_ptr<Creature>> population_;

    void executeMovePhase();
    void executeHuntPhase();
    void executeAgingPhase();
    void executeBreedPhase();
    void executeDeathPhase();

    using PositionMap = std::vector<std::pair<std::pair<int, int>, std::vector<Creature*>>>;
    PositionMap groupCreaturesByCell() const;
};