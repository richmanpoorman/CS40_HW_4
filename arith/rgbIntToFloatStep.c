/* 
 *   Name       : rgbIntToFloatStep.c
 *   Assignment : CS40 Homework 4 (arith)
 *   Purpose    : Module that converts RGB integer values to floats
 *   Editors    : Matthew Wong (mwong14), Ivi Fung (sfung02)
 */

#include <pnm.h>
#include <a2plain.h>
#include "CompressionStep.h"
#include <a2methods.h>  
#include "PixelStructs.h"
#include <stdio.h>
#include <assert.h>

#define intDenominator 255

static void compress(Pnm_ppm image);
static void decompress(Pnm_ppm image);

static void toFloat(int col, int row, A2Methods_UArray2 uarray2, 
                    A2Methods_Object *ptr, void *cl);
static void toInt(int col, int row, A2Methods_UArray2 uarray2, 
                  A2Methods_Object *ptr, void *cl);


/*
 *  Name      : compress
 *  Purpose   : Converts the pixels from Pnm_rgb to Rgb_float
 *  Parameters: (Pnm_ppm) image = Image with Pnm_rgb as pixels
 *  Output    : (None)
 *  Notes     : Assumes that the Pnm_ppm is in proper format, with the
 *              values being Pnm_rgb structs (no way to check :( );
 *              Will CRE if can not allocate new memory
 *              Will CRE if given a null image, or the values are null
 */
static void compress(Pnm_ppm image)
{
        assert(image != NULL);

        int width  = image -> width;
        int height = image -> height;

        A2Methods_T methods = uarray2_methods_plain;

        A2Methods_UArray2 pixels   = image -> pixels;
        assert(pixels != NULL);
        int               size     = sizeof(struct Rgb_float);
        A2Methods_UArray2 newImage = methods -> new(width, height, size);
        assert(newImage != NULL);

        methods -> map_row_major(newImage, toFloat, image);
        methods -> free(&pixels);
        
        image -> pixels      = newImage;
        image -> denominator = 1;
}

/*
 *  Name      : toFloat
 *  Purpose   : Copy the old image data into the new image data
 *              going from RGB int to RGB float
 *  Parameters: (int)                col     = The current column to copy
 *              (int)                row     = The current row to copy
 *              (A2Methods_UArray2)  uarray2 = The new array to copy into
 *              (A2Methods_Object *) ptr     = The RGB value in the new array
 *              (void *)             cl      = The package of the array with 
 *                                             RGB ints along with the 
 *                                             denominator
 *  Output    : (None)
 *  Notes     : Converts the RGB values of ints to RGB values of floats
 */
static void toFloat(int col, int row, A2Methods_UArray2 uarray2, 
                    A2Methods_Object *ptr, void *cl)
{

        A2Methods_T methods = uarray2_methods_plain;
        Pnm_ppm     image   = cl;

        assert(uarray2 != NULL);
        assert(image != NULL);

        A2Methods_UArray2 pixels      = image -> pixels;
        int               denominator = image -> denominator;

        assert(pixels != NULL);

        Pnm_rgb   data        = methods -> at(pixels, col, row);
        Rgb_float inNewImage  = ptr;
        assert(data != NULL);
        assert(inNewImage != NULL);

        struct Rgb_float newPixel = {
                intToFloat(data -> red  , denominator, 0, 1),
                intToFloat(data -> green, denominator, 0, 1),
                intToFloat(data -> blue , denominator, 0, 1)
        };
        *inNewImage = newPixel;
}


/*
 *  Name      : decompress
 *  Purpose   : Turns the pixels from Rgb_float to Pnm_rgb
 *              going from RGB float to RGB int
 *  Parameters: (Pnm_ppm) image = Image with Rgb_floats as pixels
 *  Output    : (None)
 *  Notes     : Assumes that the Pnm_ppm is in proper format, with the
 *              values being Rgb_float structs (no way to check :( );
 *              Will CRE if can not allocate new memory
 *              Will CRE if given a null image, or the values are null
 */
static void decompress(Pnm_ppm image)
{
        assert(image != NULL);

        int width  = image -> width;
        int height = image -> height;

        A2Methods_T methods = uarray2_methods_plain;

        A2Methods_UArray2 pixels   = image -> pixels;
        assert(pixels != NULL);
        int               size     = sizeof(struct Pnm_rgb);
        A2Methods_UArray2 newImage = methods -> new(width, height, size);
        assert(newImage != NULL);

        methods -> map_row_major(newImage, toInt, pixels);
        methods -> free(&pixels);
        
        image -> pixels      = newImage;
        image -> denominator = intDenominator;
}

/*
 *  Name      : toInt
 *  Purpose   : Copy the old image data into the new image data
 *  Parameters: (int)                col     = The current column to copy
 *              (int)                row     = The current row to copy
 *              (A2Methods_UArray2)  uarray2 = The new array to copy into
 *              (A2Methods_Object *) ptr     = The RGB value in the new array
 *              (void *)             cl      = The array with RGB floats
 *  Output    : (None)
 *  Notes     : Converts the RGB values of floats to RGB values of ints
 *              Uses the global variable intDenominator as the new 
 *              denominator
 */
static void toInt(int col, int row, A2Methods_UArray2 uarray2, 
                  A2Methods_Object *ptr, void *cl)
{
        
        A2Methods_T       methods = uarray2_methods_plain;
        A2Methods_UArray2 pixels  = cl;

        assert(uarray2 != NULL);
        assert(pixels != NULL);

        Rgb_float data       = methods -> at(pixels, col, row);
        Pnm_rgb   inNewImage = ptr;

        assert(data != NULL);
        assert(inNewImage != NULL);

        struct Pnm_rgb newPixel = {
                floatToInt(data -> red  , intDenominator, 0, intDenominator),
                floatToInt(data -> green, intDenominator, 0, intDenominator),
                floatToInt(data -> blue , intDenominator, 0, intDenominator)
        };
        *inNewImage = newPixel;
}

static struct CompressionStep rgbIntToFloatStruct = {
        compress,
        decompress
};

CompressionStep rgbIntToFloatStep = &rgbIntToFloatStruct;

#undef intDenominator