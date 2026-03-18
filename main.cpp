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

// ── Manual input ─────────────────────────────
void manualInput() {
    std::vector<Person> students;
    int n, numHW;

    try {
        std::cout << "\nHow many students? ";
        std::cin >> n;
        if (n <= 0) throw std::invalid_argument("Number of students must be positive.");

        std::cout << "How many homework assignments per student? ";
        std::cin >> numHW;
        if (numHW <= 0) throw std::invalid_argument("Number of HW must be positive.");

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
                if (s < 0 || s > 10) throw std::out_of_range("Score must be 0-10.");
                hw.push_back(s);
            }
            double exam;
            std::cout << "Exam score   : "; std::cin >> exam;
            if (exam < 0 || exam > 10) throw std::out_of_range("Exam must be 0-10.");
            students.emplace_back(firstName, surname, hw, exam);
        }

        char choice;
        std::cout << "\nCalculate using (A)verage or (M)edian? ";
        std::cin >> choice;
        bool useMedian = (choice == 'M' || choice == 'm');

        for (auto& p : students) p.calcFinalGrade(useMedian);
        std::sort(students.begin(), students.end(), sortByName);

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

// ── Print speed result row ────────────────────
void printRow(const std::string& container, const std::string& strategy,
              int count, double t1, double t2, double t3) {
    std::cout << std::left
              << std::setw(10) << container
              << std::setw(12) << strategy
              << std::setw(12) << count
              << std::fixed << std::setprecision(4)
              << std::setw(10) << t1
              << std::setw(10) << t2
              << std::setw(10) << t3
              << std::setw(10) << (t1+t2+t3) << "\n";
}

// ── Speed test: vector ────────────────────────
void testVector(const std::string& fname, int count) {
    // Strategy 1
    {
        std::vector<Person> s;
        double t1 = measureTime([&](){ readFromFile(fname, s); });
        double t2 = measureTime([&](){ std::sort(s.begin(), s.end(), sortByName); });
        std::string pf = "v_s1_passed_" + std::to_string(count) + ".txt";
        std::string ff = "v_s1_failed_" + std::to_string(count) + ".txt";
        double t3 = measureTime([&](){ strategy1(s, pf, ff, false); });
        printRow("vector", "Strategy1", count, t1, t2, t3);
    }
    // Strategy 2
    {
        std::vector<Person> s;
        double t1 = measureTime([&](){ readFromFile(fname, s); });
        double t2 = measureTime([&](){ std::sort(s.begin(), s.end(), sortByName); });
        std::string pf = "v_s2_passed_" + std::to_string(count) + ".txt";
        std::string ff = "v_s2_failed_" + std::to_string(count) + ".txt";
        double t3 = measureTime([&](){ strategy2(s, pf, ff, false); });
        printRow("vector", "Strategy2", count, t1, t2, t3);
    }
}

// ── Speed test: list ──────────────────────────
void testList(const std::string& fname, int count) {
    // Strategy 1
    {
        std::list<Person> s;
        double t1 = measureTime([&](){ readFromFile(fname, s); });
        double t2 = measureTime([&](){ s.sort(sortByName); });
        std::string pf = "l_s1_passed_" + std::to_string(count) + ".txt";
        std::string ff = "l_s1_failed_" + std::to_string(count) + ".txt";
        double t3 = measureTime([&](){ strategy1List(s, pf, ff, false); });
        printRow("list", "Strategy1", count, t1, t2, t3);
    }
    // Strategy 2
    {
        std::list<Person> s;
        double t1 = measureTime([&](){ readFromFile(fname, s); });
        double t2 = measureTime([&](){ s.sort(sortByName); });
        std::string pf = "l_s2_passed_" + std::to_string(count) + ".txt";
        std::string ff = "l_s2_failed_" + std::to_string(count) + ".txt";
        double t3 = measureTime([&](){ strategy2List(s, pf, ff, false); });
        printRow("list", "Strategy2", count, t1, t2, t3);
    }
}

// ── Speed test: deque ─────────────────────────
void testDeque(const std::string& fname, int count) {
    // Strategy 1
    {
        std::deque<Person> s;
        double t1 = measureTime([&](){ readFromFile(fname, s); });
        double t2 = measureTime([&](){ std::sort(s.begin(), s.end(), sortByName); });
        std::string pf = "d_s1_passed_" + std::to_string(count) + ".txt";
        std::string ff = "d_s1_failed_" + std::to_string(count) + ".txt";
        double t3 = measureTime([&](){ strategy1(s, pf, ff, false); });
        printRow("deque", "Strategy1", count, t1, t2, t3);
    }
    // Strategy 2
    {
        std::deque<Person> s;
        double t1 = measureTime([&](){ readFromFile(fname, s); });
        double t2 = measureTime([&](){ std::sort(s.begin(), s.end(), sortByName); });
        std::string pf = "d_s2_passed_" + std::to_string(count) + ".txt";
        std::string ff = "d_s2_failed_" + std::to_string(count) + ".txt";
        double t3 = measureTime([&](){ strategy2(s, pf, ff, false); });
        printRow("deque", "Strategy2", count, t1, t2, t3);
    }
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

    std::cout << "\n";
    std::cout << std::left
              << std::setw(10) << "Container"
              << std::setw(12) << "Strategy"
              << std::setw(12) << "Records"
              << std::setw(10) << "Read(s)"
              << std::setw(10) << "Sort(s)"
              << std::setw(10) << "Split(s)"
              << std::setw(10) << "Total(s)" << "\n";
    std::cout << std::string(74, '=') << "\n";

    for (int s : sizes) {
        std::string fname = "students" + std::to_string(s) + ".txt";
        std::cout << "\n-- " << s << " records --\n";
        try {
            testVector(fname, s);
            testList  (fname, s);
            testDeque (fname, s);
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
              << "  3. Run speed analysis (vector/list/deque x Strategy1/2)\n"
              << "Choice: ";
    std::cin >> option;

    switch (option) {
        case 1: manualInput();   break;
        case 2: generateFiles(); break;
        case 3: speedAnalysis(); break;
        default: std::cout << "Invalid option.\n";
    }

    return 0;
}
