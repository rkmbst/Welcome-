cmake_minimum_required(VERSION 3.22)

project(BeastEngine LANGUAGES CXX)

# ------------------------------------------------------------
# Core sources
# ------------------------------------------------------------

file(GLOB_RECURSE BEAST_CORE_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp"
)

add_library(
    beast_core
    STATIC
    ${BEAST_CORE_SOURCES}
)

# ------------------------------------------------------------
# Include paths
# ------------------------------------------------------------

target_include_directories(
    beast_core
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

# ------------------------------------------------------------
# C++ standard
# ------------------------------------------------------------

target_compile_features(
    beast_core
    PUBLIC
        cxx_std_20
)

# ------------------------------------------------------------
# Warnings
# ------------------------------------------------------------

if(MSVC)
    target_compile_options(
        beast_core
        PRIVATE
            /W4
            /permissive-
    )
else()
    target_compile_options(
        beast_core
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wconversion
            -Wsign-conversion
    )
endif()
