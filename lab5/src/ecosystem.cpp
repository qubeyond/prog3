#include "ecosystem.hpp"

#include <algorithm>
#include <iostream>
#include <map>

#include "carnivore.hpp"
#include "herbivore.hpp"

Ecosystem::Ecosystem(int w, int h, int steps)
    : fieldWidth_(w), fieldHeight_(h), totalSteps_(steps) {
}

void Ecosystem::introduceCreature(std::unique_ptr<Creature> c) {
    population_.push_back(std::move(c));
}

Ecosystem::PositionMap Ecosystem::groupCreaturesByCell() const {
    std::map<std::pair<int, int>, std::vector<Creature*>> cellMap;
    for (auto& creature : population_) {
        cellMap[{creature->x(), creature->y()}].push_back(creature.get());
    }
    return {cellMap.begin(), cellMap.end()};
}

void Ecosystem::executeMovePhase() {
    for (auto& creature : population_) {
        creature->updatePosition(fieldWidth_, fieldHeight_);
    }
}

void Ecosystem::executeHuntPhase() {
    auto cellGroups = groupCreaturesByCell();
    std::vector<Creature*> toRemove;

    for (auto& [coords, creatures] : cellGroups) {
        Carnivore* hunter = nullptr;
        std::vector<Herbivore*> prey;

        for (auto* c : creatures) {
            if (c->kind() == CreatureKind::CARNIVORE && !hunter)
                hunter = static_cast<Carnivore*>(c);
            else if (c->kind() == CreatureKind::HERBIVORE)
                prey.push_back(static_cast<Herbivore*>(c));
        }

        if (hunter && !prey.empty()) {
            for (auto* p : prey) {
                hunter->consumePrey();
                toRemove.push_back(p);
            }
        }
    }

    population_.erase(std::remove_if(population_.begin(), population_.end(),
                                     [&toRemove](const auto& c) {
                                         return std::find(toRemove.begin(), toRemove.end(),
                                                          c.get()) != toRemove.end();
                                     }),
                      population_.end());
}

void Ecosystem::executeAgingPhase() {
    for (auto& creature : population_) creature->growOlder();
}

void Ecosystem::executeBreedPhase() {
    std::vector<std::unique_ptr<Creature>> offspring;
    for (auto& creature : population_) {
        if (creature->readyToBreed()) {
            offspring.push_back(creature->createOffspring());
            if (creature->kind() == CreatureKind::CARNIVORE)
                static_cast<Carnivore*>(creature.get())->resetHunger();
            else
                static_cast<Herbivore*>(creature.get())->onReproduced();
        }
    }
    for (auto& baby : offspring) population_.push_back(std::move(baby));
}

void Ecosystem::executeDeathPhase() {
    population_.erase(std::remove_if(population_.begin(), population_.end(),
                                     [](const auto& c) { return c->hasExpired(); }),
                      population_.end());
}

void Ecosystem::simulate(bool showProgress) {
    if (showProgress) displayState(0);
    for (int step = 1; step <= totalSteps_; ++step) {
        executeMovePhase();
        executeHuntPhase();
        executeAgingPhase();
        executeBreedPhase();
        executeDeathPhase();
        if (showProgress) displayState(step);
    }
}

void Ecosystem::displayState(int stepNum) const {
    auto cellGroups = groupCreaturesByCell();
    std::cout << "\nШаг #" << stepNum << " | Популяция: " << population_.size() << "\n";

    for (int y = 0; y < fieldHeight_; ++y) {
        for (int x = 0; x < fieldWidth_; ++x) {
            char symbol = '.';
            for (const auto& [coords, creatures] : cellGroups) {
                if (coords.first == x && coords.second == y) {
                    bool hasW = false, hasR = false;
                    for (auto* c : creatures) {
                        if (c->kind() == CreatureKind::CARNIVORE)
                            hasW = true;
                        else
                            hasR = true;
                    }
                    symbol = (hasW && hasR) ? 'X' : (hasW ? 'W' : 'R');
                }
            }
            std::cout << symbol << " ";
        }
        std::cout << "\n";
    }
}

void Ecosystem::displayResult() const {
    auto cellGroups = groupCreaturesByCell();
    for (int y = 0; y < fieldHeight_; ++y) {
        for (int x = 0; x < fieldWidth_; ++x) {
            std::string cell = "*";
            for (const auto& [coords, creatures] : cellGroups) {
                if (coords.first == x && coords.second == y) {
                    int w = 0, r = 0;
                    for (auto* c : creatures) {
                        if (c->kind() == CreatureKind::CARNIVORE)
                            ++w;
                        else
                            ++r;
                    }
                    if (w && r)
                        cell = "+" + std::to_string(r) + "-" + std::to_string(w);
                    else if (w)
                        cell = "-" + std::to_string(w);
                    else
                        cell = "+" + std::to_string(r);
                }
            }
            std::cout << cell << "\t";
        }
        std::cout << "\n";
    }
}