/* 
 *   Name       : rgbtoCieStep.c
 *   Assignment : CS40 Homework 4 (arith)
 *   Purpose    : Module that converts RGB float values to the CIE color space
 *   Editors    : Matthew Wong (mwong14), Ivi Fung (sfung02)
 */

#include <pnm.h>
#include <a2plain.h>
#include "CompressionStep.h"
#include <a2methods.h>  
#include "PixelStructs.h"
#include <stdio.h>
#include <assert.h>

#define rToY  0.299
#define gToY  0.587
#define bToY  0.114
#define rToPb -0.168736
#define gToPb -0.331264
#define bToPb 0.5
#define rToPr 0.5
#define gToPr -0.418688
#define bToPr -0.081312

#define yToR  1.0
#define yToG  1.0
#define yToB  1.0
#define pbToR 0.0
#define pbToG -0.344136
#define pbToB 1.772
#define prToR 1.402
#define prToG -0.714136
#define prToB 0.0

static void compress(Pnm_ppm image);
static void decompress(Pnm_ppm image);

static void toCie(int col, int row, A2Methods_UArray2 uarray2, 
                  A2Methods_Object *ptr, void *cl);
static void toRgb(int col, int row, A2Methods_UArray2 uarray2, 
                  A2Methods_Object *ptr, void *cl);

/*
 *  Name      : compress
 *  Purpose   : Convert the RGB float image into a CIE float image
 *  Parameters: (Pnm_ppm) image = Image with Rgb_floats as pixels
 *  Output    : (None)
 *  Notes     : Assumes that the Pnm_ppm is in proper format, with the
 *              values being Rgb_float structs (no way to check :( );
 *              Will CRE if can not allocate new memory to create the new
 *              CIE image (if necessary)
 *              Will CRE if given a null image, or the values are null
 */
static void compress(Pnm_ppm image)
{
        assert(image != NULL);

        int width  = image -> width;
        int height = image -> height;

        A2Methods_T methods = uarray2_methods_plain;

        A2Methods_UArray2 pixels      = image -> pixels;
        assert(pixels != NULL);
        int               size        = sizeof(struct Cie_float);
        A2Methods_UArray2 newImage    = methods -> new(width, height, size);
        assert(newImage != NULL);
        
        methods -> map_row_major(newImage, toCie, pixels);
        methods -> free(&pixels);
        
        image -> pixels = newImage;
}

/*
 *  Name      : toCie
 *  Purpose   : Copy the old image data into the new image data
 *              going from RGB float to CIE float
 *  Parameters: (int)                col     = The current column to copy
 *              (int)                row     = The current row to copy
 *              (A2Methods_UArray2)  uarray2 = The new array to copy into
 *              (A2Methods_Object *) ptr     = The CIE value in the new array
 *              (void *)             cl      = The RGB float image
 *  Output    : (None)
 *  Notes     : Converts the RGB float to CIE float
 */
static void toCie(int col, int row, A2Methods_UArray2 uarray2, 
                    A2Methods_Object *ptr, void *cl)
{

        A2Methods_T       methods = uarray2_methods_plain;
        A2Methods_UArray2 pixels  = cl;

        assert(uarray2 != NULL);
        assert(pixels != NULL);

        Rgb_float data       = methods -> at(pixels, col, row);
        Cie_float inNewImage = ptr;

        assert(data != NULL);
        assert(inNewImage != NULL);

        float r = data -> red;
        float g = data -> green;
        float b = data -> blue;

        float y  = rToY  * r + gToY  * g + bToY  * b;
        float pb = rToPb * r + gToPb * g + bToPb * b;
        float pr = rToPr * r + gToPr * g + bToPr * b;
        struct Cie_float newPixel = {
                clampToRange(y,   0.0, 1.0),
                clampToRange(pb, -0.5, 0.5),
                clampToRange(pr, -0.5, 0.5)
        };
        *inNewImage = newPixel;
}


/*
 *  Name      : decompress
 *  Purpose   : Turn the CIE float image into a RGB float image
 *  Parameters: (Pnm_ppm) image = Image with Cie_floats as pixels
 *  Output    : (None)
 *  Notes     : Assumes that the Pnm_ppm is in proper format, with the
 *              values being Cie_float structs (no way to check :( );
 *              Will CRE if can not allocate new memory to create the new
 *              RGB image (if necessary)
 *              Will CRE if given a null image, or the values are null
 */
static void decompress(Pnm_ppm image)
{
        assert(image != NULL);

        int width  = image -> width;
        int height = image -> height;

        A2Methods_T methods = uarray2_methods_plain;

        A2Methods_UArray2 pixels      = image -> pixels;
        assert(pixels != NULL);
        int               size        = sizeof(struct Rgb_float);
        A2Methods_UArray2 newImage    = methods -> new(width, height, size);
        assert(newImage != NULL);

        methods -> map_row_major(newImage, toRgb, pixels);
        methods -> free(&pixels);
        
        image -> pixels = newImage;
}

/*
 *  Name      : toFloat
 *  Purpose   : Copy the old image data into the new image data 
 *              going from CIE float to RGB float
 *  Parameters: (int)                col     = The current column to copy
 *              (int)                row     = The current row to copy
 *              (A2Methods_UArray2)  uarray2 = The new array to copy into
 *              (A2Methods_Object *) ptr     = The RGB value in the new array
 *              (void *)             cl      = The CIE float image
 *  Output    : (None)
 *  Notes     : Converts the CIE float to RGB float
 */
static void toRgb(int col, int row, A2Methods_UArray2 uarray2, 
                  A2Methods_Object *ptr, void *cl)
{
        A2Methods_T       methods = uarray2_methods_plain;
        A2Methods_UArray2 pixels  = cl;

        assert(uarray2 != NULL);
        assert(pixels != NULL);

        Cie_float data       = methods -> at(pixels, col, row);
        Rgb_float inNewImage = ptr;

        assert(data != NULL);
        assert(inNewImage != NULL);

        float y  = data -> y;
        float pb = data -> pb;
        float pr = data -> pr;

        float r = yToR * y + pbToR * pb + prToR * pr;
        float g = yToG * y + pbToG * pb + prToG * pr;
        float b = yToB * y + pbToB * pb + prToB * pr;
        struct Rgb_float newPixel = {
                clampToRange(r, 0.0, 1.0),
                clampToRange(g, 0.0, 1.0),
                clampToRange(b, 0.0, 1.0)
        };
        *inNewImage = newPixel;
}


static struct CompressionStep rgbToCieStepStruct = {
        compress,
        decompress
};

CompressionStep rgbToCieStep = &rgbToCieStepStruct;

#undef rToY  
#undef gToY  
#undef bToY  
#undef rToPb 
#undef gToPb 
#undef bToPb 
#undef rToPr 
#undef gToPr 
#undef bToPr 

#undef yToR  
#undef yToG 
#undef yToB 
#undef pbToR
#undef pbToG 
#undef pbToB 
#undef prToR 
#undef prToG
#undef prToB 