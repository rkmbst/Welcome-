cmake_minimum_required(VERSION 3.22)

# ------------------------------------------------------------
# Test infrastructure
# ------------------------------------------------------------

function(beast_add_test name source)
    add_executable(
        ${name}
        ${source}
    )

    target_link_libraries(
        ${name}
        PRIVATE
            beast_core
    )

    target_include_directories(
        ${name}
        PRIVATE
            "${CMAKE_SOURCE_DIR}"
    )

    target_compile_features(
        ${name}
        PRIVATE
            cxx_std_20
    )

    add_test(
        NAME ${name}
        COMMAND ${name}
    )
endfunction()

# ------------------------------------------------------------
# Tests
# ------------------------------------------------------------

beast_add_test(
    beast_time_test
    core/time/time_test.cpp
)

beast_add_test(
    beast_add_clip_command_test
    core/commands/add_clip_command_test.cpp
)
