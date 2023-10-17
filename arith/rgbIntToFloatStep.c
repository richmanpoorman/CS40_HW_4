#include <pnm.h>
#include <a2plain.h>
#include "CompressionStep.h"
#include <a2methods.h>  

#include <stdio.h>

#define toIntDenominator 255

static void compress(Pnm_ppm image);
static void decompress(Pnm_ppm image);

static void toFloat(int col, int row, A2Methods_UArray2 uarray2, 
                    A2Methods_Object *ptr, void *cl);
static void toInt(int col, int row, A2Methods_UArray2 uarray2, 
                  A2Methods_Object *ptr, void *cl);

typedef struct RgbIntToFloatStep_intImage {
        A2Methods_UArray2 pixels;
        int denominator;
} RgbIntToFloatStep_intImage;

typedef struct Rgb_float {
        float red;
        float green;
        float blue;
} *Rgb_float;

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
#include <stdio.h>
static void compress(Pnm_ppm image)
{
        int width  = image -> width;
        int height = image -> height;

        A2Methods_T methods = uarray2_methods_plain;

        A2Methods_UArray2 pixels      = image -> pixels;
        int               size        = sizeof(struct Rgb_float);
        int               denominator = image -> denominator;
        A2Methods_UArray2 newImage    = methods -> new(width, height, size);
        
        RgbIntToFloatStep_intImage imageToFloat = {
                pixels,
                denominator
        };

        methods -> map_row_major(newImage, toFloat, &imageToFloat);
        methods -> free(&pixels);
        
        image -> pixels = newImage;
}
static void toFloat(int col, int row, A2Methods_UArray2 uarray2, 
                    A2Methods_Object *ptr, void *cl)
{

        A2Methods_T                 methods      = uarray2_methods_plain;
        RgbIntToFloatStep_intImage *imageToFloat = cl;
        A2Methods_UArray2           pixels       = imageToFloat -> pixels;
        int                         denominator  = imageToFloat -> denominator;
        Pnm_rgb                     data         = methods -> at(pixels, 
                                                                 col, row);
        Rgb_float                   inNewImage   = ptr;
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
 *  Purpose   : Do nothing on the image (no way to get trimmed back)
 *  Parameters: (Pnm_ppm) image = The original image to trim
 *  Output    : (None)
 *  Notes     : Does nothing (just to fit into struct well)
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
        
        image -> pixels = newImage;
}
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