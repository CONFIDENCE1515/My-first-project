cmake_minimum_required(VERSION 3.10)
project(StudentGradeCalculator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Console version
add_executable(grades
    main.cpp
    Person.cpp
    functions.cpp
)

# GUI version (Win32)
add_executable(grades_gui WIN32
    gui_main.cpp
)
target_link_libraries(grades_gui comdlg32)

install(TARGETS grades_gui
    DESTINATION "C:/Program Files/VVK/Name_Surname"
)
