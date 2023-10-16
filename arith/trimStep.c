#include <pnm.h>
#include <a2plain.h>
#include "CompressionStep.h"

#include <stdio.h>

void compress(Pnm_ppm image);
void decompress(Pnm_ppm image);

void trimStep_copyIntoNew(int col, int row, A2Methods_UArray2 uarray2, 
                          A2Methods_Object *ptr, void *cl);

void compress(Pnm_ppm image)
{
        int width  = image -> width;
        int height = image -> height;
        if (width % 2 == 0 && height % 2 == 0) {
                fprintf(stdout, "This is even\n");
                return;
        }

        A2Methods_T methods = uarray2_methods_plain;
        
        /* Get the even width and height */
        width  = width  - (width  % 2);
        height = height - (height % 2);
        A2Methods_UArray2 pixels   = image -> pixels;
        int               size     = methods -> size(pixels);
        A2Methods_UArray2 newImage = methods -> new(width, height, size);
        
        methods -> map_row_major(newImage, trimStep_copyIntoNew, pixels);
        methods -> free(&pixels);
        
        image -> pixels = newImage;
        image -> width  = width;
        image -> height = height;
}
void trimStep_copyIntoNew(int col, int row, A2Methods_UArray2 uarray2, 
                          A2Methods_Object *ptr, void *cl)
{
        A2Methods_T       methods       = uarray2_methods_plain;
        A2Methods_UArray2 previousImage = cl;

        Pnm_rgb inNewImage = ptr;
        Pnm_rgb data       = methods -> at(previousImage, col, row);

        *inNewImage = *data;
        (void) uarray2;
}


void decompress(Pnm_ppm image)
{
        (void) image;
}

static struct CompressionStep trimStepStruct = {
        compress,
        decompress
};

CompressionStep trimStep = &trimStepStruct;
