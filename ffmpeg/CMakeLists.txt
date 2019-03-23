
project(ffmpeg_learn)

include_directories("/usr/local/include")

set(FFMPEG_TESTS
        hello_ffmpeg
        )

foreach(TEST ${FFMPEG_TESTS})
        add_executable(${TEST} ${TEST}.cpp)
        target_link_libraries(${TEST} libavutil.so libavcodec.so libavformat.so libavdevice.so libavfilter.so libswscale.so libswresample.so)
endforeach(TEST)