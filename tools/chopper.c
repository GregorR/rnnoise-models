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

#define CHOP_SZ 384000

int main(int argc, char **argv)
{
    int *fds;
    int i;
    ssize_t rd, total;
    short buf[CHOP_SZ];

    fds = malloc((argc-1)*sizeof(int));
    if (fds == NULL) {
        perror("malloc");
        exit(1);
    }

    /* Open each file */
    for (i = 1; i < argc; i++) {
        int fd = open(argv[i], O_RDONLY);
        if (fd < 0) {
            perror(argv[i]);
            exit(1);
        }
        fds[i-1] = fd;
    }

    /* Loop over the input */
    while (1) {
        char haveInput = 0;

        /* Loop over each file */
        for (i = 0; i < argc - 1; i++) {
            int fd = fds[i];
            if (fd < 0) continue;
            haveInput = 1;

            /* Take an appropriate chunk */
            total = 0;
            while (total < sizeof(buf)) {
                rd = read(fd, ((char *) buf) + total, sizeof(buf) - total);
                if (rd <= 0) {
                    close(fd);
                    fds[i] = -1;
                    break;
                }
                total += rd;
            }
            if (total == 0 || (total%sizeof(short)) != 0)
                continue;

            /* Write it out */
            write(1, buf, total);
        }

        /* Stop if they were all ended */
        if (!haveInput)
            break;
    }

    return 0;
}
