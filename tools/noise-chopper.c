/*
 * Copyright (c) 2018 Gregor Richards
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define TOTAL_SZ (10000000LL * 480)

int main(int argc, char **argv)
{
    FILE *fh;
    int i;
    short *buf = NULL;
    long perFileSz = TOTAL_SZ / (argc-1);
    long fileSz;
    size_t rd;

    buf = malloc(perFileSz * sizeof(short));
    if (!buf) {
        perror("malloc");
        return 1;
    }

    /* Loop over the input */
    for (i = 1; i < argc; i++) {
        /* Seek to the middle */
        fh = fopen(argv[i], "rb");
        fseek(fh, 0, SEEK_END);
        fileSz = ftell(fh) / sizeof(short);
        if (fileSz > perFileSz)
            fseek(fh, (fileSz / 2 - perFileSz / 2) * sizeof(short), SEEK_SET);
        else
            fseek(fh, 0, SEEK_SET);

        /* Read it in */
        rd = fread(buf, sizeof(short), perFileSz, fh);
        fclose(fh);

        /* And write it out */
        write(1, buf, rd * sizeof(short));
    }

    return 0;
}
