#include "exam_analyzer.hpp"

#include <algorithm>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>

#include "timer_utils.hpp"

namespace {
const std::vector<std::string> SUBJECTS = {"Математика", "Русский", "Физика", "Информатика",
                                           "История"};
const std::vector<std::string> FIRST_NAMES = {"Александр", "Иван", "Анна", "Мария", "Сергей"};
const std::vector<std::string> LAST_NAMES = {"Иванов", "Петров", "Сидоров", "Смирнов", "Попов"};
}  // namespace

ExamResult::ExamResult(std::string subj, int sc) : subject(std::move(subj)), score(sc) {
}

Student::Student(std::string name, int student_age, int school_num)
    : full_name(std::move(name)), age(student_age), school_number(school_num) {
}

void Student::add_exam(std::string subject, int score) {
    exam_results.emplace_back(std::move(subject), score);
}

bool Student::has_perfect_score() const {
    return std::any_of(exam_results.begin(), exam_results.end(),
                       [](const auto& r) { return r.score == 100; });
}

std::vector<Student> generate_random_students(size_t count) {
    std::vector<Student> students;
    students.reserve(count);
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<> age_dist(16, 18), school_dist(1, 50), score_dist(40, 100),
        name_dist(0, 4), subj_dist(0, 4);

    for (size_t i = 0; i < count; ++i) {
        Student s(LAST_NAMES[name_dist(gen)] + " " + FIRST_NAMES[name_dist(gen)], age_dist(gen),
                  school_dist(gen));
        for (int j = 0; j < 3; ++j) s.add_exam(SUBJECTS[subj_dist(gen)], score_dist(gen));
        students.push_back(std::move(s));
    }
    return students;
}

std::map<int, int> find_top_schools_single(const std::vector<Student>& students) {
    std::map<int, int> stats;
    for (const auto& s : students) {
        if (s.has_perfect_score()) stats[s.school_number]++;
    }
    return stats;
}

std::map<int, int> find_top_schools_parallel(const std::vector<Student>& students,
                                             int thread_count) {
    std::map<int, int> global_stats;
    std::mutex mtx;
    std::vector<std::thread> workers;
    size_t chunk = students.size() / thread_count;

    for (int i = 0; i < thread_count; ++i) {
        size_t start = i * chunk;
        size_t end = (i == thread_count - 1) ? students.size() : (i + 1) * chunk;
        workers.emplace_back([&, start, end] {
            std::map<int, int> local_stats;
            for (size_t j = start; j < end; ++j) {
                if (students[j].has_perfect_score()) local_stats[students[j].school_number]++;
            }
            std::lock_guard lock(mtx);
            for (auto const& [school, count] : local_stats) global_stats[school] += count;
        });
    }
    for (auto& w : workers) w.join();
    return global_stats;
}

void display_top_schools(const std::map<int, int>& school_stats) {
    std::vector<std::pair<int, int>> sorted(school_stats.begin(), school_stats.end());
    std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
        if (a.second != b.second) return a.second > b.second;
        return a.first < b.first;
    });

    std::cout << "\n🏆 ТОП ШКОЛ:\n";
    size_t limit = std::min(size_t(3), sorted.size());
    for (size_t i = 0; i < limit; ++i) {
        std::cout << i + 1 << ". Школа №" << sorted[i].first << ": " << sorted[i].second << "\n";
    }
}

void execute_exam_analysis() {
    size_t count;
    int threads;
    std::cout << "Школьников: ";
    std::cin >> count;
    std::cout << "Потоков: ";
    std::cin >> threads;

    auto students = generate_random_students(count);
    TimeTracker timer;

    timer.start();
    auto s_res = find_top_schools_single(students);
    timer.stop();
    std::cout << "Single: " << timer.elapsed_milliseconds() << " ms\n";

    timer.start();
    auto p_res = find_top_schools_parallel(students, threads);
    timer.stop();
    std::cout << "Parallel: " << timer.elapsed_milliseconds() << " ms\n";

    display_top_schools(p_res);
}