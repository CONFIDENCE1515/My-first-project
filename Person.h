#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <vector>
#include <string>

class Person {
private:
    std::string firstName;
    std::string surname;
    std::vector<double> homeworkScores;
    double examScore;
    double finalGrade;

public:
    // Default constructor
    Person();

    // Parameterized constructor
    Person(const std::string& firstName, const std::string& surname,
           const std::vector<double>& homeworkScores, double examScore);

    // Copy constructor (Rule of Three)
    Person(const Person& other);

    // Assignment-copy operator (Rule of Three)
    Person& operator=(const Person& other);

    // Destructor (Rule of Three)
    ~Person();

    // Getters
    std::string getFirstName() const;
    std::string getSurname() const;
    std::vector<double> getHomeworkScores() const;
    double getExamScore() const;
    double getFinalGrade() const;

    // Calculate average of homework scores
    double calcAverage() const;

    // Calculate median of homework scores
    double calcMedian() const;

    // Calculate final grade based on average or median
    void calcFinalGrade(bool useMedian = false);

    // Overloaded input operator
    friend std::istream& operator>>(std::istream& in, Person& p);

    // Overloaded output operator
    friend std::ostream& operator<<(std::ostream& out, const Person& p);
};

#endif // PERSON_H
