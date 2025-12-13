cmake_minimum_required(VERSION 3.16)
project(preprocess_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(preprocess src/main.cpp)

# Warnings 
if (MSVC)
  target_compile_options(preprocess PRIVATE /W4)
else()
  target_compile_options(preprocess PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Optional installation
install(TARGETS preprocess RUNTIME DESTINATION bin)
