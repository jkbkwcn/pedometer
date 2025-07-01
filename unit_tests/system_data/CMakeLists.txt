cmake_minimum_required(VERSION 3.14)
project(SystemDataUnitTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------
# GoogleTest
# ------------------------------
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Sources
set(SYSTEM_DATA_SOURCES
    ${CMAKE_SOURCE_DIR}/../../components/system_data/system_data.cpp
)

# Headers
set(SYSTEM_DATA_HEADERS
    ${CMAKE_SOURCE_DIR}/../../components/system_data/include/*.hpp
)

add_library(system_data STATIC
    ${SYSTEM_DATA_SOURCES}
)

target_include_directories(system_data
    PUBLIC
        ${CMAKE_SOURCE_DIR}/../../components/system_data
        ${CMAKE_SOURCE_DIR}/../../components/system_data/include
)

# ------------------------------
# Unit tests
# ------------------------------

add_executable(system_data_test
    system_data_test.cpp
)

target_link_libraries(system_data_test
    PRIVATE
        system_data
        GTest::gtest
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(system_data_test)
