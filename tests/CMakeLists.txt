cmake_minimum_required(VERSION 3.22)

project(BeastTests LANGUAGES CXX)

add_executable(
    beast_core_smoke_test
    core/smoke_test.cpp
)

target_link_libraries(
    beast_core_smoke_test
    PRIVATE
        beast_core
)

target_compile_features(
    beast_core_smoke_test
    PRIVATE
        cxx_std_20
)

add_test(
    NAME BeastCoreSmokeTest
    COMMAND beast_core_smoke_test
)
