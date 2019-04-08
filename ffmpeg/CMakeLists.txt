
project(ffmpeg_learn)

include_directories("/usr/local/include")

find_package(SDL2 REQUIRED)


set(FFMPEG_TESTS
        hello_ffmpeg
        avio_reading
        metadata
        AVFormatContext_test
        AVCodec_test
        write_frames
        Muxing_test
        muxing
        decode_audio
        encoding_test
        )

foreach(TEST ${FFMPEG_TESTS})
        add_executable(${TEST} ${TEST}.cpp)
        target_link_libraries(${TEST} SDL2 libavutil.so libavcodec.so libavformat.so libavdevice.so libavfilter.so libswscale.so libswresample.so)
endforeach(TEST)

# FFmpeg按照Module来写。
set(FFmpegLib libavutil.so libavcodec.so libavformat.so libavdevice.so libavfilter.so libswscale.so libswresample.so)

add_executable(Demuxing_test avformat/Demuxing_test.cpp)
target_link_libraries(Demuxing_test ${FFmpegLib})

add_executable(muxing_src muxing.c)
target_link_libraries(muxing_src ${FFmpegLib} -lm)