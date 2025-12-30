#pragma once

#include <map>
#include <string>
#include <vector>

struct ExamResult {
    std::string subject;
    int score;
    ExamResult(std::string subj, int sc);
};

struct Student {
    std::string full_name;
    int age;
    int school_number;
    std::vector<ExamResult> exam_results;

    Student(std::string name, int student_age, int school_num);
    void add_exam(std::string subject, int score);
    bool has_perfect_score() const;
};

std::vector<Student> generate_random_students(size_t count);
std::map<int, int> find_top_schools_single(const std::vector<Student>& students);
std::map<int, int> find_top_schools_parallel(const std::vector<Student>& students,
                                             int thread_count);
void display_top_schools(const std::map<int, int>& school_stats);
void execute_exam_analysis();