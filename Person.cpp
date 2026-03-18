#include "Person.h"
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <iomanip>

// Default constructor
Person::Person() : firstName(""), surname(""), examScore(0.0), finalGrade(0.0) {}

// Parameterized constructor
Person::Person(const std::string& firstName, const std::string& surname,
               const std::vector<double>& homeworkScores, double examScore)
    : firstName(firstName), surname(surname),
      homeworkScores(homeworkScores), examScore(examScore), finalGrade(0.0) {}

// Copy constructor (Rule of Three)
Person::Person(const Person& other)
    : firstName(other.firstName), surname(other.surname),
      homeworkScores(other.homeworkScores), examScore(other.examScore),
      finalGrade(other.finalGrade) {}

// Assignment-copy operator (Rule of Three)
Person& Person::operator=(const Person& other) {
    if (this != &other) {
        firstName = other.firstName;
        surname = other.surname;
        homeworkScores = other.homeworkScores;
        examScore = other.examScore;
        finalGrade = other.finalGrade;
    }
    return *this;
}

// Destructor (Rule of Three)
Person::~Person() {
    // std::vector cleans itself up automatically
}

// Getters
std::string Person::getFirstName() const { return firstName; }
std::string Person::getSurname() const { return surname; }
std::vector<double> Person::getHomeworkScores() const { return homeworkScores; }
double Person::getExamScore() const { return examScore; }
double Person::getFinalGrade() const { return finalGrade; }

// Calculate average of homework scores
double Person::calcAverage() const {
    if (homeworkScores.empty()) return 0.0;
    double sum = std::accumulate(homeworkScores.begin(), homeworkScores.end(), 0.0);
    return sum / homeworkScores.size();
}

// Calculate median of homework scores
double Person::calcMedian() const {
    if (homeworkScores.empty()) return 0.0;
    std::vector<double> sorted = homeworkScores;
    std::sort(sorted.begin(), sorted.end());
    size_t n = sorted.size();
    if (n % 2 == 0)
        return (sorted[n / 2 - 1] + sorted[n / 2]) / 2.0;
    else
        return sorted[n / 2];
}

// Calculate final grade using average or median for homework
// Formula: Final = 0.4 * (Average or Median of HW) + 0.6 * Exam
void Person::calcFinalGrade(bool useMedian) {
    double hwComponent = useMedian ? calcMedian() : calcAverage();
    finalGrade = 0.4 * hwComponent + 0.6 * examScore;
}

// Overloaded >> operator for data input
std::istream& operator>>(std::istream& in, Person& p) {
    in >> p.firstName >> p.surname;

    p.homeworkScores.clear();
    double score;

    // Read homework scores until "exam" keyword or end of line
    // For interactive input: user enters scores one by one, -1 to stop
    std::cout << "Enter homework scores one by one (-1 to finish): ";
    while (in >> score && score != -1) {
        p.homeworkScores.push_back(score);
    }

    std::cout << "Enter exam score: ";
    in >> p.examScore;

    return in;
}

// Overloaded << operator for data output
std::ostream& operator<<(std::ostream& out, const Person& p) {
    out << std::left
        << std::setw(12) << p.firstName
        << std::setw(12) << p.surname
        << std::fixed << std::setprecision(2)
        << std::setw(10) << p.finalGrade;
    return out;
}
