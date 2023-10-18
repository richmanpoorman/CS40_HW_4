#include <pnm.h>
#include <a2plain.h>
#include "CompressionStep.h"
#include <a2methods.h>  
#include "PixelStructs.h"
#include <stdio.h>

#define toIntDenominator 255

static void compress(Pnm_ppm image);
static void decompress(Pnm_ppm image);

static void toFloat(int col, int row, A2Methods_UArray2 uarray2, 
                    A2Methods_Object *ptr, void *cl);
static void toInt(int col, int row, A2Methods_UArray2 uarray2, 
                  A2Methods_Object *ptr, void *cl);


/*
 *  Name      : compress
 *  Purpose   : Turns the pixels from Pnm_rgb to Rgb_float
 *  Parameters: (Pnm_ppm) image = The original image convert format
 *  Output    : (None)
 *  Notes     : Assumes that the Pnm_ppm is in proper format, with the
 *              values being Pnm_rgb structs (no way to check :( );
 *              Will CRE if can not allocate new memory
 */
static void compress(Pnm_ppm image)
{
        int width  = image -> width;
        int height = image -> height;

        A2Methods_T methods = uarray2_methods_plain;

        A2Methods_UArray2 pixels      = image -> pixels;
        int               size        = sizeof(struct Rgb_float);
        A2Methods_UArray2 newImage    = methods -> new(width, height, size);
        

        methods -> map_row_major(newImage, toFloat, image);
        methods -> free(&pixels);
        
        image -> pixels      = newImage;
        image -> denominator = 1;
}

/*
 *  Name      : toFloat
 *  Purpose   : Copy the old image data into the new image data
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

        A2Methods_T       methods     = uarray2_methods_plain;
        Pnm_ppm           image       = cl;
        A2Methods_UArray2 pixels      = image -> pixels;
        int               denominator = image -> denominator;
        Pnm_rgb           data        = methods -> at(pixels, col, row);
        Rgb_float         inNewImage  = ptr;
        struct Rgb_float newPixel = {
                1.0 * (data -> red)   / denominator,
                1.0 * (data -> green) / denominator,
                1.0 * (data -> blue)  / denominator
        };
        *inNewImage = newPixel;
        (void) uarray2;
}


/*
 *  Name      : decompress
 *  Purpose   : Turns the pixels from Rgb_float to Pnm_rgb
 *  Parameters: (Pnm_ppm) image = The original image convert format
 *  Output    : (None)
 *  Notes     : Assumes that the Pnm_ppm is in proper format, with the
 *              values being Pnm_rgb structs (no way to check :( );
 *              Will CRE if can not allocate new memory
 */
static void decompress(Pnm_ppm image)
{
        int width  = image -> width;
        int height = image -> height;

        A2Methods_T methods = uarray2_methods_plain;

        A2Methods_UArray2 pixels      = image -> pixels;
        int               size        = sizeof(struct Pnm_rgb);
        A2Methods_UArray2 newImage    = methods -> new(width, height, size);

        methods -> map_row_major(newImage, toInt, pixels);
        methods -> free(&pixels);
        
        image -> pixels      = newImage;
        image -> denominator = toIntDenominator;
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
 *              Uses the global variable toIntDenominator as the new 
 *              denominator
 */
static void toInt(int col, int row, A2Methods_UArray2 uarray2, 
                  A2Methods_Object *ptr, void *cl)
{
        A2Methods_T       methods     = uarray2_methods_plain;
        A2Methods_UArray2 pixels      = cl;
        Rgb_float         data        = methods -> at(pixels, col, row);
        Pnm_rgb           inNewImage  = ptr;

        struct Pnm_rgb newPixel = {
                (int)((data -> red)   * toIntDenominator),
                (int)((data -> green) * toIntDenominator),
                (int)((data -> blue)  * toIntDenominator)
        };
        *inNewImage = newPixel;
        (void) uarray2;
}

static struct CompressionStep rgbIntToFloatStruct = {
        compress,
        decompress
};

CompressionStep rgbIntToFloatStep = &rgbIntToFloatStruct;

#undef toIntDenominator