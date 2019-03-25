
project(sdl_learn)

SET(SDL_TESTS
        res_path
        hello_sdl
        multi_function
        extension_lib
        )

foreach(TEST ${SDL_TESTS})
    add_executable(${TEST} ${TEST}.cpp)
    target_link_libraries(${TEST} SDL2)
endforeach(TEST)
