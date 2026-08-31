cmake_minimum_required(VERSION 3.22)

project(
    BeastMediaStudio
    VERSION 0.1.0
    LANGUAGES CXX
)

# ------------------------------------------------------------
# Language
# ------------------------------------------------------------

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(
    CMAKE_POSITION_INDEPENDENT_CODE
    ON
)

# ------------------------------------------------------------
# Options
# ------------------------------------------------------------

option(
    BEAST_BUILD_TESTS
    "Build Beast tests"
    ON
)

# ------------------------------------------------------------
# Engine
# ------------------------------------------------------------

add_subdirectory(
    native/beast_engine
)

# ------------------------------------------------------------
# Tests
# ------------------------------------------------------------

if(BEAST_BUILD_TESTS)

    enable_testing()

    add_subdirectory(
        tests
    )

endif()
