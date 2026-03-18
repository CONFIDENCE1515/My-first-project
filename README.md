# Student Grade Calculator

A C++ application to calculate student final grades based on homework and exam scores.

## Final Grade Formula

```
Final Points = 0.4 * (Average | Median of HW) + 0.6 * Exam
```

## Features

- Manual student data entry (unknown number of homework scores supported)
- Read student data from `Students.txt`
- Random score generation for testing
- Calculates final grade using **Average** or **Median** of homework scores
- Students sorted by name/surname
- Nicely formatted table output

## Project Structure

```
├── main.cpp       # Main program & menu logic
├── Person.h       # Person class declaration
├── Person.cpp     # Person class implementation
├── Students.txt   # Sample student data file
└── README.md      # This file
```

## How to Compile

```bash
g++ -o grades main.cpp Person.cpp -std=c++17
```

## How to Run

```bash
./grades
```

Then choose from the menu:
1. **Manual input** – enter students interactively
2. **Read from file** – loads `Students.txt`
3. **Random generation** – generates random homework/exam scores

## Students.txt Format

```
Name Surname HW1 HW2 HW3 ... HWn Exam
```

Example:
```
Alice Johnson 9 8 10 7 9 8
Bob Smith 6 7 5 8 6 7
```

## Output Example

```
Name        Surname     Final (Avg.)  | Final (Med.)
----------------------------------------------------
Alice       Johnson     8.60          | 8.60
Bob         Smith       6.80          | 6.80
```

## OOP Design

The `Person` class follows the **Rule of Three**:
- Copy constructor
- Assignment-copy operator
- Destructor

Data input/output uses overloaded `>>` and `<<` operators.
