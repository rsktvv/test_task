cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME test)
project("${PROJECT_NAME}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Создаем цель для копирования файла Input.txt
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Input.txt
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/Input.txt
        ${CMAKE_CURRENT_BINARY_DIR}/Input.txt
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Input.txt
    COMMENT "Copying Input.txt to build directory"
)

# Создаем цель, которая зависит от копирования файлов
add_custom_target(
    copy_input_output_files
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Input.txt 
)

# Добавляем исполняемый файл, зависящий от копирования файлов
add_executable("${PROJECT_NAME}" test.cpp Input.txt )
add_dependencies("${PROJECT_NAME}" copy_input_output_files)