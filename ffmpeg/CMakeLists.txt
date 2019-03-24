
project(ffmpeg_learn)

include_directories("/usr/local/include")

find_package(SDL2 REQUIRED)


set(FFMPEG_TESTS
        hello_ffmpeg
        avio_reading
        metadata
        AVFormatContext_test
        write_frames
        )

foreach(TEST ${FFMPEG_TESTS})
        add_executable(${TEST} ${TEST}.cpp)
        target_link_libraries(${TEST} SDL2 libavutil.so libavcodec.so libavformat.so libavdevice.so libavfilter.so libswscale.so libswresample.so)
endforeach(TEST)