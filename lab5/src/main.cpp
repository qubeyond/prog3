#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <string>

#include "carnivore.hpp"
#include "ecosystem.hpp"
#include "herbivore.hpp"

namespace {
MoveDirection intToDir(int val) {
    return static_cast<MoveDirection>(std::clamp(val, 0, 3));
}

int readInt(const std::string& prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= minVal && value <= maxVal) return value;
        std::cout << "Ошибка: введите число от " << minVal << " до " << maxVal << "\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

bool askYesNo(const std::string& prompt) {
    std::cout << prompt;
    char ch;
    std::cin >> ch;
    return (ch == 'y' || ch == 'Y' || ch == 'd' || ch == 'D');
}

void generateRandom(Ecosystem& eco, int w, int h, int numHerbivores, int numCarnivores) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<> distX(0, w - 1), distY(0, h - 1), distDir(0, 3), distK(1, 5);

    for (int i = 0; i < numHerbivores; ++i)
        eco.introduceCreature(std::make_unique<Herbivore>(distX(rng), distY(rng),
                                                          intToDir(distDir(rng)), distK(rng)));
    for (int i = 0; i < numCarnivores; ++i)
        eco.introduceCreature(std::make_unique<Carnivore>(distX(rng), distY(rng),
                                                          intToDir(distDir(rng)), distK(rng)));
}

void loadFromStream(std::istream& in, Ecosystem& eco) {
    int numPrey, numPred;
    if (!(in >> numPrey >> numPred)) return;

    auto load = [&](int count, bool isHerbivore) {
        for (int i = 0; i < count; ++i) {
            int x, y, d, k;
            in >> x >> y >> d >> k;
            if (isHerbivore)
                eco.introduceCreature(std::make_unique<Herbivore>(x, y, intToDir(d), k));
            else
                eco.introduceCreature(std::make_unique<Carnivore>(x, y, intToDir(d), k));
        }
    };
    load(numPrey, true);
    load(numPred, false);
}

void showMenu() {
    std::cout << "\n=== СИМУЛЯЦИЯ ЭКОСИСТЕМЫ ===\n"
              << "1. Загрузить из файла\n"
              << "2. Ручной ввод данных\n"
              << "3. Случайная генерация\n"
              << "4. Тест #1 (3x3, 3 хода)\n"
              << "5. Тест #2 (4x4, 20 ходов)\n"
              << "0. Выход\n"
              << "Выбор: ";
}
}  // namespace

int main(int argc, char* argv[]) {
    if (argc > 1) {
        bool visual = false;
        std::string path;
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if ((arg == "-v" || arg == "--visual"))
                visual = true;
            else if ((arg == "-f" || arg == "--file") && i + 1 < argc)
                path = argv[++i];
        }

        std::ifstream file(path);
        if (!file) return (std::cerr << "Ошибка файла\n", 1);

        int N, M, T;
        file >> N >> M >> T;
        Ecosystem eco(N, M, T);
        loadFromStream(file, eco);
        eco.simulate(visual);
        eco.displayResult();
        return 0;
    }

    while (true) {
        showMenu();
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (choice == 0) break;

        int N, M, T;
        if (choice == 1) {
            std::cout << "Путь: ";
            std::string p;
            std::cin >> p;
            std::ifstream f(p);
            if (!f) {
                std::cout << "Ошибка!\n";
                continue;
            }
            f >> N >> M >> T;
            Ecosystem eco(N, M, T);
            loadFromStream(f, eco);
            eco.simulate(askYesNo("Пошагово? (y/n): "));
            eco.displayResult();
        } else if (choice == 2 || choice == 3) {
            N = readInt("Ширина: ", 1, 100);
            M = readInt("Высота: ", 1, 100);
            T = readInt("Ходы: ", 1, 500);
            int R = readInt("Жертвы: ", 0, 50), W = readInt("Хищники: ", 0, 50);
            Ecosystem eco(N, M, T);
            bool visual = askYesNo("Пошагово? (y/n): ");
            if (choice == 3)
                generateRandom(eco, N, M, R, W);
            else {
                for (int i = 0; i < R; ++i) {
                    std::cout << "R#" << i + 1 << " (x y d k): ";
                    int x, y, d, k;
                    std::cin >> x >> y >> d >> k;
                    eco.introduceCreature(std::make_unique<Herbivore>(x, y, intToDir(d), k));
                }
                for (int i = 0; i < W; ++i) {
                    std::cout << "W#" << i + 1 << " (x y d k): ";
                    int x, y, d, k;
                    std::cin >> x >> y >> d >> k;
                    eco.introduceCreature(std::make_unique<Carnivore>(x, y, intToDir(d), k));
                }
            }
            eco.simulate(visual);
            eco.displayResult();
        } else if (choice == 4) {
            Ecosystem eco(3, 3, 3);
            eco.introduceCreature(std::make_unique<Herbivore>(1, 2, MoveDirection::EAST, 1));
            eco.introduceCreature(std::make_unique<Herbivore>(1, 1, MoveDirection::NORTH, 2));
            eco.introduceCreature(std::make_unique<Carnivore>(0, 2, MoveDirection::EAST, 2));
            eco.simulate(askYesNo("Пошагово? (y/n): "));
            eco.displayResult();
        } else if (choice == 5) {
            Ecosystem eco(4, 4, 20);
            eco.introduceCreature(std::make_unique<Herbivore>(0, 0, MoveDirection::EAST, 100));
            eco.introduceCreature(std::make_unique<Carnivore>(0, 3, MoveDirection::NORTH, 100));
            eco.simulate(askYesNo("Пошагово? (y/n): "));
            eco.displayResult();
        }

        std::cout << "\n[Enter для продолжения]";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
    return 0;
}