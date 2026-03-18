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
#include "Person.h"

// Sort comparator
inline bool sortByName(const Person& a, const Person& b) {
    return (a.getFirstName() != b.getFirstName())
        ? a.getFirstName() < b.getFirstName()
        : a.getSurname()   < b.getSurname();
}

// Generate a random student data file
void generateStudentFile(const std::string& filename, int count);

// Measure execution time
template<typename Func>
double measureTime(Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

// Read from file into a container
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

// Write one person to output file
inline void writePerson(std::ofstream& out, const Person& p) {
    out << std::left
        << std::setw(12) << p.getFirstName()
        << std::setw(12) << p.getSurname()
        << std::fixed << std::setprecision(2)
        << p.getFinalGrade() << "\n";
}

// Split vector/deque containers
template<typename Container>
void splitStudents(Container& students,
                   const std::string& passedFile,
                   const std::string& failedFile,
                   bool useMedian) {
    for (auto& p : students)
        p.calcFinalGrade(useMedian);

    std::sort(students.begin(), students.end(), sortByName);

    std::ofstream passed(passedFile);
    std::ofstream failed(failedFile);
    if (!passed.is_open()) throw std::runtime_error("Cannot open: " + passedFile);
    if (!failed.is_open()) throw std::runtime_error("Cannot open: " + failedFile);

    for (const auto& p : students) {
        if (p.getFinalGrade() >= 5.0) writePerson(passed, p);
        else                          writePerson(failed, p);
    }
}

// Split for std::list
inline void splitStudentsList(std::list<Person>& students,
                               const std::string& passedFile,
                               const std::string& failedFile,
                               bool useMedian) {
    for (auto& p : students)
        p.calcFinalGrade(useMedian);

    students.sort(sortByName);

    std::ofstream passed(passedFile);
    std::ofstream failed(failedFile);
    if (!passed.is_open()) throw std::runtime_error("Cannot open: " + passedFile);
    if (!failed.is_open()) throw std::runtime_error("Cannot open: " + failedFile);

    for (const auto& p : students) {
        if (p.getFinalGrade() >= 5.0) writePerson(passed, p);
        else                          writePerson(failed, p);
    }
}

#endif // FUNCTIONS_H
