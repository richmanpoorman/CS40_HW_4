#include <pnm.h>
#include <a2plain.h>
#include "CompressionStep.h"

#include <stdio.h>

static void compress(Pnm_ppm image);
static void decompress(Pnm_ppm image);

// static void toFloat(int col, int row, A2Methods_UArray2 uarray2, 
//                     A2Methods_Object *ptr, void *cl);
// static void toInt(int col, int row, A2Methods_UArray2 uarray2, 
//                   A2Methods_Object *ptr, void *cl);
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
        (void) image;
        // int width  = image -> width;
        // int height = image -> height;
        // if (width % 2 == 0 && height % 2 == 0) {
        //         return;
        // }

        // A2Methods_T methods = uarray2_methods_plain;
        
        // /* Get the even width and height */
        // width  = width  - (width  % 2);
        // height = height - (height % 2);
        // A2Methods_UArray2 pixels   = image -> pixels;
        // int               size     = methods -> size(pixels);
        // A2Methods_UArray2 newImage = methods -> new(width, height, size);
        
        // methods -> map_row_major(newImage, trimStep_copyIntoNew, pixels);
        // methods -> free(&pixels);
        
        // image -> pixels = newImage;
        // image -> width  = width;
        // image -> height = height;
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

static struct CompressionStep rgbIntToFloatStruct = {
        compress,
        decompress
};

CompressionStep rgbIntToFloatStep = &rgbIntToFloatStruct;
