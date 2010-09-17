/*
 * This file shows how to use io_png.c. It is released in the public
 * domain and as such comes with no copyright requirement.
 *
 * compile with : cc io_tiff_example.c io_tiff.c -ltiff
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* include the io_png prototypes */
#include "io_tiff.h"

int main()
{
    /*
     * number of columns, lines and channels of the image
     * size_t is the safe type,
     * because the image size is used as an array index
     */

    size_t nx, ny, nc;

    /*
     * image array,
     * initially set to NULL for safety
     */

    float *img = NULL;

    /* read the image */
    img = read_tiff_f32_gray("in.tiff", &nx, &ny);
    nc = 1;

    /* if img == NULL, there was an error while reading */
    if (NULL == img)
    {
        fprintf(stderr, "failed to read the image in.tiff\n");
        abort();
    }

    /* nx, ny and nc hols the image sizes */
    printf("image size : %i x %i, %i channels\n",
           (int) nx, (int) ny, (int) nc);

    {
        /* let's access the pixel (27, 42) */
        size_t x = 27;
        size_t y = 42;
        printf("the pixel (%i, %i) is %f\n",
               (int) x, (int) y, img[x + nx * y]);
    }

    /* write the image */
    if (0 != write_tiff_f32("out.tiff", img, nx, ny, nc))
    {
        fprintf(stderr, "failed to write the image out.tiff\n");
        abort();
    }
    free(img);

    return EXIT_SUCCESS;
}

/**
 * @mainpage io_tiff : simplified front-end to libtiff
 *
 * README.txt:
 * @verbinclude README.txt
 */
