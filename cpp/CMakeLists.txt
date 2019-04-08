

project(cpp_learn)

set(CPP_API_TESTS
        unique_ptr_test
        optional_test
        two_pointers
)

foreach(TEST ${CPP_API_TESTS})
    add_executable(${TEST} ${TEST}.cpp)
endforeach(TEST)