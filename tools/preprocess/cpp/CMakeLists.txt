cmake_minimum_required(VERSION 3.16)
project(preprocess_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
	preprocess
	src/main.cpp
	src/provenance.cpp
    # Add more source files as needed
)

target_include_directories(preprocess PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)

# Warnings 
if (MSVC)
  target_compile_options(preprocess PRIVATE /W4)
else()
  target_compile_options(preprocess PRIVATE -Wall -Wextra -Wpedantic)
endif()

find_package(OpenSSL QUIET)

if (OpenSSL_FOUND)
    target_link_libraries(preprocess PRIVATE OpenSSL::SSL OpenSSL::Crypto)
    target_compile_definitions(preprocess PRIVATE HLS_PREPROCESS_HAVE_OPENSSL=1)
endif()

# Optional installation
install(TARGETS preprocess RUNTIME DESTINATION bin)
