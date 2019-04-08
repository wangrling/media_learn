//
// Created by wangrl on 19-4-7.
//

/**
 * A simple test program to test libpng.
 *
 * This program reads in a PNG image, writes it out again, and then compares the two files.
 * If the files are identical, this shows that the basic chunk handling, filtering, and (de)compression
 * code is working properly. It does not currently test all of the transforms, although
 * it probably should.
 *
 * The program will report "FAIL" in certain legitimate cases:
 * 1) when the compression level or filter selection method is changed.
 * 2) When the maximum IDAT size (PNG_ZBUF_SIZE in pngconf.h) is not 8192.
 * 3) unknown unsafe-to-copy ancillary chunks or unknown critical chunks
 * exist in the input file.
 * 4) others not listed here...
 * In these cases, it is best to check with another tool such as "pngcheck"
 * to see what the differences between the two files are.
 *
 * If a filename is given on the command-line, then this file is used for the input,
 * rather than the default "pngtest.png". This allows testing a wide varity of
 * files easily. You can also test a number of files at once by typing
 * "pngtest -m file1.png file2.png ..."
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <png.h>
#include <zlib.h>

#define STDERR stdout

static int verbose = 0;
static int strict = 0;
static int relaxed = 0;
static int xfail = 0;


/* Example of using row callbacks to make a simple progress meter */
static int status_pass = 1;
static int status_dots_requested = 0;
static int status_dots = 1;

static const char *inname = "pngtest.png";
static const char *outname = "pngout.png";

int main(int argc, char* argv[]) {

    int multiple = 0;
    int ierror = 0;

    png_structp dummy_ptr;

    fprintf(STDERR, "\n Testing libpng version %s\n", PNG_LIBPNG_VER_STRING);
    fprintf(STDERR, " with zlib version %s\n", ZLIB_VERSION);

    /* Show the version of libpng used in building the library */
    fprintf(STDERR, " library (%lu):%s",
            (unsigned long) png_access_version_number(),
            png_get_header_version(NULL));

    if (strcmp(png_libpng_ver, PNG_LIBPNG_VER_STRING)) {
        fprintf(STDERR,
                "Warning: versions are different between png.h and png.c\n");
        fprintf(STDERR, " png.h version: %s\n", PNG_LIBPNG_VER_STRING);
        fprintf(STDERR, " png.c version: %s\n\n", png_libpng_ver);
    }

    if (argc > 1) {
        if (strcmp(argv[1], "-m") == 0) {
            multiple = 1;
            status_dots_requested = 0;
        } else if (strcmp(argv[1], "-mv") == 0 ||
                strcmp(argv[1], "-vm") == 0) {
            multiple = 1;
            verbose = 1;
            status_dots_requested = 1;
        } else if (strcmp(argv[1], "v") == 0) {
            verbose = 1;
            status_dots_requested = 1;
            inname = argv[2];
        } else if (strcmp(argv[1], "--strict") == 0) {
            status_dots_requested = 0;
            verbose = 1;
            inname = argv[2];
            strict++;
            relaxed = 0;
            multiple = 1;
        } else if (strcmp(argv[1], "--relaxed") == 0) {
            status_dots_requested = 0;
            verbose = 1;
            inname = argv[2];
            strict = 0;
            relaxed++;
            multiple = 1;
        } else if (strcmp(argv[1], "--xfail") == 0) {
            status_dots_requested = 0;
            verbose = 1;
            inname = argv[2];
            strict = 0;
            xfail++;
            relaxed++;
            multiple = 1;
        } else {
            inname = argv[1];
            status_dots_requested = 0;
        }
    }

    return 0;
}

