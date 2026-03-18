#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include "Person.h"

// ─────────────────────────────────────────────
// Sort helper
// ─────────────────────────────────────────────
bool sortByName(const Person& a, const Person& b) {
    if (a.getFirstName() != b.getFirstName())
        return a.getFirstName() < b.getFirstName();
    return a.getSurname() < b.getSurname();
}

// ─────────────────────────────────────────────
// MAIN
// ─────────────────────────────────────────────
int main() {
    std::cout << "====================================\n";
    std::cout << "    Student Grade Calculator    \n";
    std::cout << "====================================\n";

    std::vector<Person> students;
    int n, numHW;

    std::cout << "\nHow many students? ";
    std::cin >> n;
    std::cout << "How many homework assignments per student? ";
    std::cin >> numHW;

    // Enter each student
    for (int i = 0; i < n; ++i) {
        std::cout << "\n--- Student " << (i + 1) << " ---\n";
        std::string firstName, surname;
        std::cout << "First name   : "; std::cin >> firstName;
        std::cout << "Surname      : "; std::cin >> surname;

        std::vector<double> hw;
        for (int j = 1; j <= numHW; ++j) {
            double s;
            std::cout << "HW" << j << " score    : ";
            std::cin >> s;
            hw.push_back(s);
        }

        double exam;
        std::cout << "Exam score   : "; std::cin >> exam;

        students.emplace_back(firstName, surname, hw, exam);
    }

    // Choose calculation method
    char choice;
    std::cout << "\nCalculate final grade using (A)verage or (M)edian? ";
    std::cin >> choice;
    bool useMedian = (choice == 'M' || choice == 'm');

    // Calculate grades
    for (auto& p : students)
        p.calcFinalGrade(useMedian);

    // Sort by name
    std::sort(students.begin(), students.end(), sortByName);

    // Print results table
    std::cout << "\n========== RESULTS (" << (useMedian ? "Median" : "Average") << ") ==========\n";
    std::cout << std::left
              << std::setw(12) << "Name"
              << std::setw(12) << "Surname";
    for (int i = 1; i <= numHW; ++i)
        std::cout << std::setw(6) << ("HW" + std::to_string(i));
    std::cout << std::setw(8) << "Exam"
              << std::setw(12) << "Final" << "\n";
    std::cout << std::string(12 + 12 + numHW * 6 + 8 + 12, '-') << "\n";

    for (auto& p : students) {
        std::cout << std::left << std::fixed << std::setprecision(2)
                  << std::setw(12) << p.getFirstName()
                  << std::setw(12) << p.getSurname();
        for (double hw : p.getHomeworkScores())
            std::cout << std::setw(6) << hw;
        std::cout << std::setw(8) << p.getExamScore()
                  << std::setw(12) << p.getFinalGrade() << "\n";
    }
    std::cout << std::string(12 + 12 + numHW * 6 + 8 + 12, '=') << "\n";

    return 0;
}
