# Student Grade Calculator

A C++ application to calculate student final grades based on homework and exam scores.

## Final Grade Formula

```
Final Points = 0.4 * (Average | Median of HW) + 0.6 * Exam
```

## Project Structure

```
├── main.cpp        # Main program & menu logic
├── Person.h        # Person class declaration
├── Person.cpp      # Person class implementation
├── functions.h     # Utility templates (read, split, timer, strategies)
├── functions.cpp   # File generation
├── Students.txt    # Sample student data file
└── README.md       # This file
```

## How to Compile

```bash
g++ -o grades main.cpp Person.cpp functions.cpp -std=c++17
```

## How to Run

```bash
./grades        # Linux/Mac
.\grades.exe    # Windows
```

## Menu Options

| Option | Description |
|--------|-------------|
| 1 | Manual input — enter student names and scores |
| 2 | Generate 4 random data files (10K/100K/1M/10M records) |
| 3 | Speed analysis — compare containers and strategies |

## Splitting Strategies

### Strategy 1 (std::copy_if)
- Copies passed/failed students into **two NEW containers**
- Original container keeps ALL students
- Uses `std::copy_if` from `<algorithm>`
- More memory usage but non-destructive

### Strategy 2 (std::stable_partition)
- Uses `std::stable_partition` to split in-place
- Failed students are **removed** from original container
- Only passed students remain in original
- More memory efficient

## Containers Tested

- `std::vector` — fast random access, best cache performance
- `std::list`   — fast insert/delete, uses own `.sort()`
- `std::deque`  — double-ended queue, good for both ends

## Release Notes

### v0.1
- Manual student input
- Rule of Three (Person class)
- Average and Median calculation
- Sorted output table

### v0.2
- Exception handling (try/catch)
- File generation (10K–10M students)
- Speed measurement
- vector / list / deque containers

### v0.25
- Strategy 1 and Strategy 2 splitting
- Full speed comparison table

### v1.0
- Final release
- Algorithm library functions: std::copy_if, std::stable_partition, std::sort
- Full speed analysis across all containers and strategies
- CMakeLists.txt support
- Complete README with usage guide

## Application Usage Guide

1. Compile: `g++ -o grades main.cpp Person.cpp functions.cpp -std=c++17`
2. Run: `.\grades.exe`
3. Choose option 2 first to generate data files
4. Then choose option 3 to run speed analysis
5. Results show Read / Sort / Split times per container and strategy

## Results Summary

Strategy 2 (stable_partition) is generally faster and more memory efficient.
Vector consistently outperforms list and deque for large datasets.
