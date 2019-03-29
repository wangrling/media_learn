
project(sdl_learn)


SET(SDL_TESTS
        res_path
        hello_sdl
        multi_function
        extension_lib
        handle_events
        clipping_sprite
        sdl_font
        )

foreach(TEST ${SDL_TESTS})
    add_executable(${TEST} ${TEST}.cpp)
    target_link_libraries(${TEST} SDL2 SDL2_image)
endforeach(TEST)
