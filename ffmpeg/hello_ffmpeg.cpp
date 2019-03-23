#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

static void logging(const char* fmt, ...);

/**
 * Decode packets into frames.
 */
static int decode_packet(AVPacket* pPacket, AVCodecContext* pCodecContext, AVFrame* pFrame);

// save a frame into a .pgm file
static void save_gray_frame(unsigned char *buf, int wrap, int xsize, int ysize, char *filename);


int main(int argc, char **argv) {

    logging("initializing all the containers, codecs and protocols.");
    /**
     * AVFormatContext holds the header information from the format (Container)
     * Allocating memory for this component.
     */
    AVFormatContext* pFormatContext = avformat_alloc_context();
    if (!pFormatContext) {
        logging("ERROR could not allocate memory for Format Context.");
        return -1;
    }

    logging(
            "opening the input file (%s) and loading format (container) header.",
            argv[1]);
    /**
     * Open the file and read its header. The codecs are not opened.
     */
    if (avformat_open_input(&pFormatContext, argv[1], NULL, NULL) != 0) {
        logging("ERROR could not open the file.");
        return -1;
    }

    /**
     * Now we have access to some information about our file since we read its header
     * we can say what format (container) it's and some other information related to the
     * format itself.
     */
    logging("format %s, duration %lld us, bit_rate %lld.",
            pFormatContext->iformat->name, pFormatContext->duration,
            pFormatContext->bit_rate);

    logging("finding stream info from format.");
    /**
     * Read Packets from the Format to get stream information, this function populates  pFormatContext->streams.
     */
    if (avformat_find_stream_info(pFormatContext, NULL) < 0) {
        logging("ERROR could not get the stream info.");
        return -1;
    }

    /**
     * The component that knows how to encode and decode the stream
     * it's the codec (audio or video).
     */
    AVCodec* pCodec = NULL;

    /**
     * This component describes the properties of a codec used by the stream i..
     */
    AVCodecParameters* pCodecParameters = NULL;

    int video_stream_index = -1;

    /**
     * Loop through all the stream and print its main information.
     */
    for (int i = 0; i < pFormatContext->nb_streams; i++) {
        AVCodecParameters* pLocalCodecParameters = NULL;
        pLocalCodecParameters = pFormatContext->streams[i]->codecpar;

        logging("AVStream->time_base before open coded %d/%d",
                pFormatContext->streams[i]->time_base.num,
                pFormatContext->streams[i]->time_base.den);
        logging("AVStream->r_frame_rate before open coded %d/%d",
                pFormatContext->streams[i]->r_frame_rate.num,
                pFormatContext->streams[i]->r_frame_rate.den);
        logging("AVStream->start_time %" PRId64,
                pFormatContext->streams[i]->start_time);
        logging("AVStream->duration %" PRId64,
                pFormatContext->streams[i]->duration);

        logging("finding the proper decoder (CODEC)");

        AVCodec* pLocalCodec = NULL;

        /**
         * Finds the registered decoder for a codec ID.
         */
        pLocalCodec = avcodec_find_decoder(pLocalCodecParameters->codec_id);

        if (pLocalCodec == NULL) {
            logging("ERROR unsupported codec!");
            return -1;
        }

        /**
         * When the stream is a video we store its index, codec paramters and codec.
         */
        if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_index = i;
            pCodec = pLocalCodec;
            pCodecParameters = pLocalCodecParameters;

            logging("Video Codec: resolution %d x %d",
                    pLocalCodecParameters->width,
                    pLocalCodecParameters->height);
        } else if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_AUDIO) {
            logging("Audio Codec: %d channels, sample rate %d",
                    pLocalCodecParameters->channels,
                    pLocalCodecParameters->sample_rate);
        }

        // Print its name, id and bitrate.
        logging("\tCodec %s ID %d bit_rate %lld", pLocalCodec->name,
                pLocalCodec->id, pCodecParameters->bit_rate);
    }

    AVCodecContext* pCodecContext = avcodec_alloc_context3(pCodec);
    if (!pCodecContext) {
        logging("failed to allocated memory for AVCodecContext");
        return -1;
    }

    /**
     * Fill the codec context based on the values from the supplied codec parameters.
     */
    if (avcodec_parameters_to_context(pCodecContext, pCodecParameters) < 0) {
        logging("Failed to copy codec params to codec context.");
        return -1;
    }

    /**
     * Initialize the AVCodecContext to use the given AVCodec.
     */
    if (avcodec_open2(pCodecContext, pCodec, NULL) < 0) {
        logging("failed to open codec through avcodec_open2");
        return -1;
    }

    AVPacket* pPacket = av_packet_alloc();
    if (!pPacket)
    {
        logging("failed to allocated memory for AVPacket");
        return -1;
    }

    AVFrame* pFrame = av_frame_alloc();
    if (!pFrame)
    {
        logging("failed to allocated memory for AVFrame");
        return -1;
    }

    int response = 0;
    int how_many_packets_to_process = 8;

    /**
     * Fill the Packet with data from the Stream.
     */
    while (av_read_frame(pFormatContext, pPacket) >= 0) {
        // If it's the video stream.
        if (pPacket->stream_index == video_stream_index) {
            logging("AVPacket->pts %" PRId64, pPacket->pts);
            response = decode_packet(pPacket, pCodecContext, pFrame);
            if (response < 0)
                break;
            // Stop it, otherwise we'll be saving hundreds of frames.
            if (--how_many_packets_to_process <= 0)
                break;
        }
        av_packet_unref(pPacket);
    }

    logging("releasing all the resources.");

    avformat_close_input(&pFormatContext);
    avformat_free_context(pFormatContext);
    av_packet_free(&pPacket);
    av_frame_free(&pFrame);
    avcodec_free_context(&pCodecContext);

    return 0;
}

static void logging(const char *fmt, ...) {
    va_list args;
    fprintf( stderr, "LOG: ");
    va_start(args, fmt);
    vfprintf( stderr, fmt, args);
    va_end(args);
    fprintf( stderr, "\n");
}

static int decode_packet(AVPacket *pPacket, AVCodecContext *pCodecContext, AVFrame *pFrame)
{
    // Supply raw packet data as input to a decoder
    // https://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga58bc4bf1e0ac59e27362597e467efff3
    int response = avcodec_send_packet(pCodecContext, pPacket);

    if (response < 0) {
        // logging("Error while sending a packet to the decoder: %s", av_err2str(response));
        return response;
    }

    while (response >= 0)
    {
        // Return decoded output data (into a frame) from a decoder
        // https://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga11e6542c4e66d3028668788a1a74217c
        response = avcodec_receive_frame(pCodecContext, pFrame);
        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
            break;
        } else if (response < 0) {
            // logging("Error while receiving a frame from the decoder: %s", av_err2str(response));
            return response;
        }

        if (response >= 0) {
            logging(
                    "Frame %d (type=%c, size=%d bytes) pts %d key_frame %d [DTS %d]",
                    pCodecContext->frame_number,
                    av_get_picture_type_char(pFrame->pict_type),
                    pFrame->pkt_size,
                    pFrame->pts,
                    pFrame->key_frame,
                    pFrame->coded_picture_number
            );

            char frame_filename[1024];
            snprintf(frame_filename, sizeof(frame_filename), "%s-%d.pgm", "frame", pCodecContext->frame_number);
            // save a grayscale frame into a .pgm file
            save_gray_frame(pFrame->data[0], pFrame->linesize[0], pFrame->width, pFrame->height, frame_filename);

            av_frame_unref(pFrame);
        }
    }
    return 0;
}

static void save_gray_frame(unsigned char* buf, int wrap, int xsize, int ysize, char* filename) {
    FILE* f;
    int i;
    f = fopen(filename, "w");

    // Writing the minimal required header for a pgm file format.
    // portable graymap format -> https://en.wikipedia.org/wiki/Netpbm_format#PGM_example

    fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);

    // writing line by line
    for (int i = 0; i < ysize; i++) {
        fwrite(buf +  i * wrap, 1, xsize, f);
    }

    fclose(f);
}