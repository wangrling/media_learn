extern "C" {
#include <libavformat/avformat.h>

}

/**
 * libavformat API example.
 *
 * Output a media file in any supported libavformat format.
 * The default codecs are used.
 */

// a wrapper around a single output AVStream
typedef struct OutputStream {
    AVStream *st;
    AVCodecContext *enc;

    /* pts of the next frame that will be generated */
    int64_t next_pts;
    int samples_count;

    AVFrame *frame;
    AVFrame *tmp_frame;

    float t, tincr, tincr2;

    struct SwsContext *sws_ctx;
    struct SwrContext *swr_ctx;
} OutputStream;

int main(int argc, char* argv[]) {

    OutputStream video_st = {0}, audio_st = {0};
    const char *filename;
    AVOutputFormat *fmt;
    AVFormatContext *oc;
    AVCodec *audio_codec, *video_codec;

    int ret;
    int have_video = 0, have_audio = 0;
    int encode_video = 0, encode_audio = 0;
    AVDictionary *opt = NULL;
    int i;

    if (argc < 2) {
        printf("usage: %s output_file\n"
               "API example program to output a media file with libavformat.\n"
               "This program generates a synthetic audio and video stream, encodes and\n"
               "muxes them into a file named output_file.\n"
               "The output format is automatically guessed according to the file extension.\n"
               "Raw images can also be output by using '%%d' in the filename.\n"
               "\n", argv[0]);
        return 1;
    }

    filename = argv[1];
    for (i = 2; i+1 < argc; i+= 2) {
        if (!strcmp(argv[i], "-flags") || !strcmp(argv[i], "-fflags"))
            av_dict_set(&opt, argv[i]+1, argv[i+1], 0);
    }


    return 0;
}

