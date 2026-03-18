#include "functions.h"
#include <cstdlib>
#include <ctime>

void generateStudentFile(const std::string& filename, int count) {
    std::ofstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot create file: " + filename);

    for (int i = 1; i <= count; ++i) {
        file << "Name" << i << " Surname" << i;
        // 5 homework scores (0-10)
        for (int j = 0; j < 5; ++j)
            file << " " << (rand() % 11);
        // exam score (0-10)
        file << " " << (rand() % 11) << "\n";
    }
}
