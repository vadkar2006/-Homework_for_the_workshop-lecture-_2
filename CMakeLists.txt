cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(MyApp 
    main.cpp
    Test/Test.cpp
    Vector/Vector.cpp
    Task/Task.cpp
    Solving_Metod/Solving_Metod.cpp
)

target_link_options(MyApp PRIVATE -static)

# Папка include уже подключена (из вашего лога)
target_include_directories(MyApp PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Добавьте корень проекта, чтобы работало #include "Vector/Vector.h"
target_include_directories(MyApp PRIVATE ${CMAKE_SOURCE_DIR})