#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include "Person.h"
#include "functions.h"

// ── Manual input (from v0.1) ─────────────────
void manualInput() {
    std::vector<Person> students;
    int n, numHW;

    try {
        std::cout << "\nHow many students? ";
        std::cin >> n;
        if (n <= 0) throw std::invalid_argument("Number of students must be positive.");

        std::cout << "How many homework assignments per student? ";
        std::cin >> numHW;
        if (numHW <= 0) throw std::invalid_argument("Number of homework assignments must be positive.");

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
                if (s < 0 || s > 10) throw std::out_of_range("Score must be between 0 and 10.");
                hw.push_back(s);
            }

            double exam;
            std::cout << "Exam score   : "; std::cin >> exam;
            if (exam < 0 || exam > 10) throw std::out_of_range("Exam score must be between 0 and 10.");

            students.emplace_back(firstName, surname, hw, exam);
        }

        char choice;
        std::cout << "\nCalculate using (A)verage or (M)edian? ";
        std::cin >> choice;
        bool useMedian = (choice == 'M' || choice == 'm');

        for (auto& p : students)
            p.calcFinalGrade(useMedian);

        std::sort(students.begin(), students.end(), sortByName);

        // Print results
        std::cout << "\n========== RESULTS (" << (useMedian ? "Median" : "Average") << ") ==========\n";
        std::cout << std::left
                  << std::setw(12) << "Name"
                  << std::setw(12) << "Surname";
        for (int i = 1; i <= numHW; ++i)
            std::cout << std::setw(6) << ("HW" + std::to_string(i));
        std::cout << std::setw(8) << "Exam" << std::setw(12) << "Final" << "\n";
        std::cout << std::string(12 + 12 + numHW * 6 + 20, '-') << "\n";

        for (auto& p : students) {
            std::cout << std::left << std::fixed << std::setprecision(2)
                      << std::setw(12) << p.getFirstName()
                      << std::setw(12) << p.getSurname();
            for (double hw : p.getHomeworkScores())
                std::cout << std::setw(6) << hw;
            std::cout << std::setw(8) << p.getExamScore()
                      << std::setw(12) << p.getFinalGrade() << "\n";
        }
        std::cout << std::string(12 + 12 + numHW * 6 + 20, '=') << "\n";

    } catch (const std::exception& e) {
        std::cerr << "\nError: " << e.what() << "\n";
    }
}

// ── Run speed test with a container type ─────
template<typename Container>
void runSpeedTest(const std::string& label,
                  const std::string& filename,
                  int count) {
    std::cout << "\n[" << label << "] " << count << " records\n";

    Container students;

    // 1. Read
    double t1 = measureTime([&]() {
        readFromFile(filename, students);
    });
    std::cout << "  Read        : " << std::fixed << std::setprecision(4) << t1 << " s\n";

    // 2. Sort
    double t2 = measureTime([&]() {
        std::sort(students.begin(), students.end(), sortByName);
    });
    std::cout << "  Sort        : " << t2 << " s\n";

    // 3. Split
    std::string passedFile = "passed_" + label + "_" + std::to_string(count) + ".txt";
    std::string failedFile = "failed_" + label + "_" + std::to_string(count) + ".txt";
    double t3 = measureTime([&]() {
        splitStudents(students, passedFile, failedFile, false);
    });
    std::cout << "  Split+Write : " << t3 << " s\n";
    std::cout << "  TOTAL       : " << (t1 + t2 + t3) << " s\n";
}

// Specialization for std::list (no random access sort)
template<>
void runSpeedTest<std::list<Person>>(const std::string& label,
                                      const std::string& filename,
                                      int count) {
    std::cout << "\n[" << label << "] " << count << " records\n";

    std::list<Person> students;

    double t1 = measureTime([&]() {
        readFromFile(filename, students);
    });
    std::cout << "  Read        : " << std::fixed << std::setprecision(4) << t1 << " s\n";

    double t2 = measureTime([&]() {
        students.sort(sortByName);
    });
    std::cout << "  Sort        : " << t2 << " s\n";

    std::string passedFile = "passed_" + label + "_" + std::to_string(count) + ".txt";
    std::string failedFile = "failed_" + label + "_" + std::to_string(count) + ".txt";
    double t3 = measureTime([&]() {
        splitStudentsList(students, passedFile, failedFile, false);
    });
    std::cout << "  Split+Write : " << t3 << " s\n";
    std::cout << "  TOTAL       : " << (t1 + t2 + t3) << " s\n";
}

// ── Generate all 4 data files ─────────────────
void generateFiles() {
    srand(static_cast<unsigned>(time(nullptr)));
    std::vector<int> sizes = {10000, 100000, 1000000, 10000000};

    std::cout << "\nGenerating student data files...\n";
    for (int s : sizes) {
        std::string fname = "students" + std::to_string(s) + ".txt";
        try {
            std::cout << "  Creating " << fname << " ... ";
            std::cout.flush();
            generateStudentFile(fname, s);
            std::cout << "Done\n";
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
}

// ── Speed analysis ────────────────────────────
void speedAnalysis() {
    std::vector<int> sizes = {10000, 100000, 1000000, 10000000};

    std::cout << "\n======= SPEED ANALYSIS =======\n";

    for (int s : sizes) {
        std::string fname = "students" + std::to_string(s) + ".txt";

        std::cout << "\n--- " << s << " records ---";
        try {
            runSpeedTest<std::vector<Person>>("vector", fname, s);
            runSpeedTest<std::list<Person>>  ("list",   fname, s);
            runSpeedTest<std::deque<Person>> ("deque",  fname, s);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
}

// ── MAIN ──────────────────────────────────────
int main() {
    std::cout << "====================================\n";
    std::cout << "     Student Grade Calculator      \n";
    std::cout << "====================================\n";

    int option;
    std::cout << "\nChoose option:\n"
              << "  1. Manual input\n"
              << "  2. Generate student data files\n"
              << "  3. Run speed analysis (vector/list/deque)\n"
              << "Choice: ";
    std::cin >> option;

    switch (option) {
        case 1: manualInput();    break;
        case 2: generateFiles();  break;
        case 3: speedAnalysis();  break;
        default: std::cout << "Invalid option.\n";
    }

    return 0;
}
