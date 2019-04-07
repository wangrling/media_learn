project(png_learn)

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})

set(PNG_TESTS
    png_test
        )

foreach(TEST ${PNG_TESTS})
    add_executable(${TEST} ${TEST}.cpp)
    target_link_libraries(${TEST} ${PNG_LIBRARY})
endforeach(TEST)