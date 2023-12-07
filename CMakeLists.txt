cmake_minimum_required(VERSION 3.14)

# задаем используемый стандарт С++
set(CMAKE_CXX_STANDARD 17)

# TODO: переименовать проект
project(task1)


set(exe_name ${PROJECT_NAME})

add_executable(${exe_name} main.c "Native.c" "Native.h" "Vector.c" "Vector.h")