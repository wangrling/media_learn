//
// Created by wangrl on 19-4-4.
//
extern "C" {
#include <libavformat/avformat.h>
}

#include <iostream>
using std::cout;
using std::endl;

/**
 * Demuxers read a media file and split it into chunks of data (packets).
 *
 * A packet contains ne or more encoded frames which belongs to a single elementary stream.
 * In the lavf API this process is represented by the avformat_open_input() function for opening a file,
 * av_read_frame() for reading a single packet and finally avformat_close_input(), which does the cleanup.
 */
int main(int argc, char* argv[]) {

    const char* url = "/home/wangrl/Videos/small_bunny_1080p_30fps.mp4";
    AVFormatContext* avFormatContext = NULL;

    /**
     * Attempts to allocate an AVFormatContext, open the specified file (autodetecting the format) and read the header,
     * exporting the information stored there into avFormatContext. Some formats don't have a header or do not store
     * enough information there, so it is recommended that you call the avformat_find_stream_info() function which tries
     * to read and decode a few frames to find missing information.
     *
     * In some cases you might want to preallocate an AVFormatContext yourself with avformat_alloc_context() and do
     * some tweaking (调整) on it before passing it to avformat_open_input(). One such case is when you want to use
     * custom functions for reading input data instead of of lavf internal I/O layer. To do that, create your own
     * AVIOContext with avio_alloc_context(), passing your reading callbacks to it. Then set the pb field of your
     * AVFormatContext the newly created AVIOContext.
     */
    int ret = avformat_open_input(&avFormatContext, url, NULL, NULL);
    if (ret < 0)
        abort();
    else {
        fprintf(stdout, "avformat_open_input success\n");
    }

    AVPacket* avPacket;
    ret = av_read_frame(avFormatContext, avPacket);
    if (ret < 0) {
        abort();
    } else {
        fprintf(stdout, "av_read_frame success\n");
    }
    cout << avPacket->pts << endl;
    cout << avPacket->dts << endl;
    cout << avPacket->duration << endl;

    ret = av_read_frame(avFormatContext, avPacket);
    if (ret < 0) {
        abort();
    } else {
        fprintf(stdout, "av_read_frame second success\n");
    }
    cout << avPacket->pts << endl;
    cout << avPacket->dts << endl;
    cout << avPacket->duration << endl;

    return 0;
}

