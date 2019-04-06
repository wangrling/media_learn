extern "C" {
#include <libavformat/avformat.h>

}

/**
 * Muxers taken encoded data in the form of AVPacket and write it into files or other output
 * bytestreams in the specified container format.
 *
 * The main API function for muxing are avformat_write_header() for writing the file header,
 * av_write_frame()/av_interleaved_write_frame() for writing the packets and
 * av_write_trailer() for finalizing the file.
 *
 * (1) filling the various fields in the context.
 * (2) call avformat_write_header() to initialize the muxer internals and write the file header.
 * (3) repeatedly calling av_write_frame() or av_interleaved_write_frame().
 * (4) call av_write_trailer to flush any buffered packets and finalize the output file.
 * (5) close the IO context.
 */

int main(int argc, char* argv[]) {



    return 0;
}

