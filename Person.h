#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

class Person {
private:
    std::string firstName;
    std::string surname;
    std::vector<double> homeworkScores;
    double examScore;
    double finalGrade;

public:
    // Constructors (Rule of Three)
    Person();
    Person(const std::string& firstName, const std::string& surname,
           const std::vector<double>& homeworkScores, double examScore);
    Person(const Person& other);
    Person& operator=(const Person& other);
    ~Person();

    // Getters
    std::string getFirstName() const;
    std::string getSurname() const;
    std::vector<double> getHomeworkScores() const;
    double getExamScore() const;
    double getFinalGrade() const;

    // Calculations
    double calcAverage() const;
    double calcMedian() const;
    void calcFinalGrade(bool useMedian = false);

    // I/O operators
    friend std::istream& operator>>(std::istream& in, Person& p);
    friend std::ostream& operator<<(std::ostream& out, const Person& p);
};

#endif // PERSON_H
