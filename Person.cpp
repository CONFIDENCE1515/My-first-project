#include "Person.h"
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <stdexcept>

Person::Person() : firstName(""), surname(""), examScore(0.0), finalGrade(0.0) {}

Person::Person(const std::string& fn, const std::string& sn,
               const std::vector<double>& hw, double exam)
    : firstName(fn), surname(sn), homeworkScores(hw),
      examScore(exam), finalGrade(0.0) {}

Person::Person(const Person& other)
    : firstName(other.firstName), surname(other.surname),
      homeworkScores(other.homeworkScores), examScore(other.examScore),
      finalGrade(other.finalGrade) {}

Person& Person::operator=(const Person& other) {
    if (this != &other) {
        firstName     = other.firstName;
        surname       = other.surname;
        homeworkScores = other.homeworkScores;
        examScore     = other.examScore;
        finalGrade    = other.finalGrade;
    }
    return *this;
}

Person::~Person() {}

std::string Person::getFirstName()  const { return firstName; }
std::string Person::getSurname()    const { return surname; }
std::vector<double> Person::getHomeworkScores() const { return homeworkScores; }
double Person::getExamScore()       const { return examScore; }
double Person::getFinalGrade()      const { return finalGrade; }

double Person::calcAverage() const {
    if (homeworkScores.empty())
        throw std::runtime_error("No homework scores to average.");
    double sum = std::accumulate(homeworkScores.begin(), homeworkScores.end(), 0.0);
    return sum / homeworkScores.size();
}

double Person::calcMedian() const {
    if (homeworkScores.empty())
        throw std::runtime_error("No homework scores to compute median.");
    std::vector<double> sorted = homeworkScores;
    std::sort(sorted.begin(), sorted.end());
    size_t n = sorted.size();
    return (n % 2 == 0) ? (sorted[n/2-1] + sorted[n/2]) / 2.0 : sorted[n/2];
}

void Person::calcFinalGrade(bool useMedian) {
    try {
        double hw = useMedian ? calcMedian() : calcAverage();
        finalGrade = 0.4 * hw + 0.6 * examScore;
    } catch (const std::exception& e) {
        finalGrade = 0.0;
    }
}

std::istream& operator>>(std::istream& in, Person& p) {
    in >> p.firstName >> p.surname >> p.examScore;
    return in;
}

std::ostream& operator<<(std::ostream& out, const Person& p) {
    out << std::left
        << std::setw(12) << p.firstName
        << std::setw(12) << p.surname
        << std::fixed << std::setprecision(2)
        << std::setw(10) << p.finalGrade;
    return out;
}
