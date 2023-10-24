/* 
 *   Name       : trimStep.c
 *   Assignment : CS40 Homework 4 (arith)
 *   Purpose    : Module that trims an input image to have even dimensions
 *   Editors    : Matthew Wong (mwong14), Ivi Fung (sfung02)
 */
 
#include <pnm.h>
#include <a2plain.h>
#include "CompressionStep.h"

#include <stdio.h>

static void compress(Pnm_ppm image);
static void decompress(Pnm_ppm image);

static void copyIntoNew(int col, int row, A2Methods_UArray2 uarray2, 
                                 A2Methods_Object *ptr, void *cl);

/*
 *  Name      : compress
 *  Purpose   : Trim the edges to make the dimensions even by even
 *  Parameters: (Pnm_ppm) image = The original image to trim
 *  Output    : (None)
 *  Notes     : Assumes that the Pnm_ppm is in proper format, with the
 *              values being Pnm_rgb structs (no way to check :( );
 *              Will CRE if can not allocate new memory to create the new
 *              trimmed image (if necessary)
 */
static void compress(Pnm_ppm image)
{
        int width  = image -> width;
        int height = image -> height;
        if (width % 2 == 0 && height % 2 == 0) {
                return;
        }

        A2Methods_T methods = uarray2_methods_plain;
        
        /* Get the even width and height */
        width  = width  - (width  % 2);
        height = height - (height % 2);
        A2Methods_UArray2 pixels   = image -> pixels;
        int               size     = methods -> size(pixels);
        A2Methods_UArray2 newImage = methods -> new(width, height, size);
        
        methods -> map_row_major(newImage, copyIntoNew, pixels);
        methods -> free(&pixels);
        
        image -> pixels = newImage;
        image -> width  = width;
        image -> height = height;
}

/*
 *  Name      : copyIntoNew
 *  Purpose   : Copy the old image data into the new image data
 *  Parameters: (int)                col     = The current column to copy
 *              (int)                row     = The current row to copy
 *              (A2Methods_UArray2)  uarray2 = The new array to copy into
 *              (A2Methods_Object *) ptr     = The RGB value in the new array
 *              (void *)             cl      = The old pixel array to copy from
 *  Output    : (None)
 *  Notes     : Excludes information that is not in the new image 
 *              (i.e. the trimmed columns/rows will be excluded)
 */
static void copyIntoNew(int col, int row, A2Methods_UArray2 uarray2, 
                                 A2Methods_Object *ptr, void *cl)
{
        A2Methods_T       methods       = uarray2_methods_plain;
        A2Methods_UArray2 previousImage = cl;

        Pnm_rgb inNewImage = ptr;
        Pnm_rgb data       = methods -> at(previousImage, col, row);

        *inNewImage = *data;
        (void) uarray2;
}

/*
 *  Name      : decompress
 *  Purpose   : Do nothing on the image (no way to get trimmed back)
 *  Parameters: (Pnm_ppm) image = The original image to trim
 *  Output    : (None)
 *  Notes     : Does nothing (just to fit into struct well)
 */
static void decompress(Pnm_ppm image)
{
        (void) image;
}

static struct CompressionStep trimStepStruct = {
        compress,
        decompress
};

CompressionStep trimStep = &trimStepStruct;
