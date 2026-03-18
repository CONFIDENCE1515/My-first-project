#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <deque>
#include <string>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <stdexcept>
#include <functional>
#include "Person.h"

// ── Sort comparator ──────────────────────────
inline bool sortByName(const Person& a, const Person& b) {
    return (a.getFirstName() != b.getFirstName())
        ? a.getFirstName() < b.getFirstName()
        : a.getSurname()   < b.getSurname();
}

// ── Generate a random student data file ──────
void generateStudentFile(const std::string& filename, int count);

// ── Measure execution time ───────────────────
template<typename Func>
double measureTime(Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

// ── Read from file into a container ──────────
template<typename Container>
void readFromFile(const std::string& filename, Container& students) {
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file: " + filename);

    students.clear();
    std::string line;
    int lineNum = 0;

    while (std::getline(file, line)) {
        ++lineNum;
        if (line.empty()) continue;
        try {
            std::istringstream ss(line);
            std::string fn, sn;
            std::vector<double> all;
            double val;
            ss >> fn >> sn;
            while (ss >> val) all.push_back(val);
            if (all.empty())
                throw std::runtime_error("No scores on line " + std::to_string(lineNum));
            double exam = all.back();
            all.pop_back();
            students.emplace_back(fn, sn, all, exam);
        } catch (const std::exception& e) {
            std::cerr << "Warning: skipping line " << lineNum << " - " << e.what() << "\n";
        }
    }
}

// ── Write one person to output file ──────────
inline void writePerson(std::ofstream& out, const Person& p) {
    out << std::left
        << std::setw(12) << p.getFirstName()
        << std::setw(12) << p.getSurname()
        << std::fixed << std::setprecision(2)
        << p.getFinalGrade() << "\n";
}

// ─────────────────────────────────────────────
// STRATEGY 1:
// Copy students into TWO new containers (passed/failed)
// Original container keeps ALL students
// Uses std::copy_if from <algorithm>
// ─────────────────────────────────────────────
template<typename Container>
void strategy1(Container& students,
               const std::string& passedFile,
               const std::string& failedFile,
               bool useMedian) {
    // Calculate grades
    for (auto& p : students)
        p.calcFinalGrade(useMedian);

    // Sort original
    std::sort(students.begin(), students.end(), sortByName);

    // Copy into two new containers using std::copy_if
    Container passed, failed;

    std::copy_if(students.begin(), students.end(),
                 std::back_inserter(passed),
                 [](const Person& p){ return p.getFinalGrade() >= 5.0; });

    std::copy_if(students.begin(), students.end(),
                 std::back_inserter(failed),
                 [](const Person& p){ return p.getFinalGrade() < 5.0; });

    // Write to files
    std::ofstream pf(passedFile), ff(failedFile);
    if (!pf.is_open()) throw std::runtime_error("Cannot open: " + passedFile);
    if (!ff.is_open()) throw std::runtime_error("Cannot open: " + failedFile);

    for (const auto& p : passed) writePerson(pf, p);
    for (const auto& p : failed) writePerson(ff, p);
}

// Strategy 1 for std::list
inline void strategy1List(std::list<Person>& students,
                           const std::string& passedFile,
                           const std::string& failedFile,
                           bool useMedian) {
    for (auto& p : students)
        p.calcFinalGrade(useMedian);

    students.sort(sortByName);

    std::list<Person> passed, failed;
    std::copy_if(students.begin(), students.end(),
                 std::back_inserter(passed),
                 [](const Person& p){ return p.getFinalGrade() >= 5.0; });
    std::copy_if(students.begin(), students.end(),
                 std::back_inserter(failed),
                 [](const Person& p){ return p.getFinalGrade() < 5.0; });

    std::ofstream pf(passedFile), ff(failedFile);
    if (!pf.is_open()) throw std::runtime_error("Cannot open: " + passedFile);
    if (!ff.is_open()) throw std::runtime_error("Cannot open: " + failedFile);

    for (const auto& p : passed) writePerson(pf, p);
    for (const auto& p : failed) writePerson(ff, p);
}

// ─────────────────────────────────────────────
// STRATEGY 2:
// Move failed students OUT of original container
// into a "failed" container (original keeps only passed)
// Uses std::stable_partition from <algorithm>
// ─────────────────────────────────────────────
template<typename Container>
void strategy2(Container& students,
               const std::string& passedFile,
               const std::string& failedFile,
               bool useMedian) {
    for (auto& p : students)
        p.calcFinalGrade(useMedian);

    std::sort(students.begin(), students.end(), sortByName);

    // Partition: passed students come first
    auto boundary = std::stable_partition(
        students.begin(), students.end(),
        [](const Person& p){ return p.getFinalGrade() >= 5.0; }
    );

    // Write passed (before boundary)
    std::ofstream pf(passedFile);
    if (!pf.is_open()) throw std::runtime_error("Cannot open: " + passedFile);
    for (auto it = students.begin(); it != boundary; ++it)
        writePerson(pf, *it);

    // Write failed (from boundary to end)
    std::ofstream ff(failedFile);
    if (!ff.is_open()) throw std::runtime_error("Cannot open: " + failedFile);
    for (auto it = boundary; it != students.end(); ++it)
        writePerson(ff, *it);

    // Remove failed from original (only passed remain)
    students.erase(boundary, students.end());
}

// Strategy 2 for std::list
inline void strategy2List(std::list<Person>& students,
                           const std::string& passedFile,
                           const std::string& failedFile,
                           bool useMedian) {
    for (auto& p : students)
        p.calcFinalGrade(useMedian);

    students.sort(sortByName);

    auto boundary = std::stable_partition(
        students.begin(), students.end(),
        [](const Person& p){ return p.getFinalGrade() >= 5.0; }
    );

    std::ofstream pf(passedFile);
    if (!pf.is_open()) throw std::runtime_error("Cannot open: " + passedFile);
    for (auto it = students.begin(); it != boundary; ++it)
        writePerson(pf, *it);

    std::ofstream ff(failedFile);
    if (!ff.is_open()) throw std::runtime_error("Cannot open: " + failedFile);
    for (auto it = boundary; it != students.end(); ++it)
        writePerson(ff, *it);

    students.erase(boundary, students.end());
}

// Keep old splitStudents for compatibility
template<typename Container>
void splitStudents(Container& students,
                   const std::string& passedFile,
                   const std::string& failedFile,
                   bool useMedian) {
    strategy1(students, passedFile, failedFile, useMedian);
}

inline void splitStudentsList(std::list<Person>& students,
                               const std::string& passedFile,
                               const std::string& failedFile,
                               bool useMedian) {
    strategy1List(students, passedFile, failedFile, useMedian);
}

#endif // FUNCTIONS_H
