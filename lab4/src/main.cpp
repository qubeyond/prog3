#include <unistd.h>

#include <iostream>
#include <limits>

#include "concurrency_stress.hpp"
#include "exam_analyzer.hpp"
#include "fork_dilemma.hpp"

void show_menu() {
    std::cout << "\n=== Лабораторная 4: Параллельное программирование ===\n"
              << "1. Задача 1: Стресс-тест конкурентного доступа\n"
              << "2. Задача 2: Анализ результатов ЕГЭ\n"
              << "3. Задача 3: Симуляция обедающих мыслителей\n"
              << "0. Выход\n"
              << "Выберите опцию: ";
}

void flush_input() {
    if (!std::cin.eof()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int main() {
    int selection;

    while (true) {
        show_menu();

        if (!(std::cin >> selection)) {
            if (std::cin.eof()) return 0;
            flush_input();
            std::cout << "Ошибка: введите число.\n";
            continue;
        }

        switch (selection) {
            case 1:
                execute_concurrency_tests();
                break;
            case 2:
                execute_exam_analysis();
                break;
            case 3:
                execute_fork_dilemma();
                break;
            case 0:
                return 0;
            default:
                std::cout << "Некорректный выбор.\n";
        }

        if (selection != 0 && isatty(fileno(stdin))) {
            std::cout << "\nНажмите Enter для продолжения...";
            flush_input();
            std::cin.get();
        }
    }
}