#include <pnm.h>
#include <a2plain.h>
#include "CompressionStep.h"
#include <a2methods.h>  
#include "PixelStructs.h"
#include <stdio.h>
#include <arith40.h>


/* TODO: Changed to be based on the bits; should be moved to a 
   different file */
#define numBCDBits 4
#define numABits 9
#define maxFloat 0.3

#define maxInt (1 << numBCDBits)
#define maxAInt (1 << numABits)
#define scaleFactor (maxInt / maxFloat)

static void compress(Pnm_ppm image);
static void decompress(Pnm_ppm image);

static void quantize(int col, int row, A2Methods_UArray2 uarray2, 
                     A2Methods_Object *ptr, void *cl);
static void dequantize(int col, int row, A2Methods_UArray2 uarray2, 
                       A2Methods_Object *ptr, void *cl);

static int   bcdToBits(float x);
static float bcdToFloat(int x);

/* TODO: Added these functions to handle the different case */
static int   aToBits(float a);
static float aToFloat(int a);

/*
 *  Name      : compress
 *  Purpose   : Convert the DCT float image into a DCT int image
 *  Parameters: (Pnm_ppm) image = Image with Dct_floats as pixels
 *  Output    : (None)
 *  Notes     : Assumes that the Pnm_ppm is in proper format, with the
 *              values being Dct_float structs (no way to check :( );
 *              Will CRE if can not allocate new memory to create the new
 *              trimmed image (if necessary)
 */
static void compress(Pnm_ppm image)
{
        int width  = image -> width;
        int height = image -> height;

        A2Methods_T methods = uarray2_methods_plain;

        A2Methods_UArray2 pixels      = image -> pixels;
        int               size        = sizeof(struct Dct_int);
        A2Methods_UArray2 newImage    = methods -> new(width, height, size);
        
        methods -> map_row_major(newImage, quantize, pixels);
        methods -> free(&pixels);
        
        image -> pixels = newImage;
}

/*
 *  Name      : quantize
 *  Purpose   : Copy the old image data into the new image data
 *              going from DCT float to DCT int
 *  Parameters: (int)                col     = The current column to copy
 *              (int)                row     = The current row to copy
 *              (A2Methods_UArray2)  uarray2 = The new array to copy into
 *              (A2Methods_Object *) ptr     = The DCT value in the new array
 *              (void *)             cl      = The DCT float image
 *  Output    : (None)
 *  Notes     : Converts the RGB float to CIE float
 */
static void quantize(int col, int row, A2Methods_UArray2 uarray2, 
                     A2Methods_Object *ptr, void *cl)
{
        A2Methods_T       methods = uarray2_methods_plain;
        A2Methods_UArray2 pixels  = cl;
        Dct_float         data    = methods -> at(pixels, col, row);

        Dct_int inNewImage = ptr;
        float     a          = data -> a;
        float     b          = data -> b;
        float     c          = data -> c;
        float     d          = data -> d;
        float     pb         = data -> pb;
        float     pr         = data -> pr;
        struct Dct_int newPixel = {
                aToBits(a), /*TODO: FIX THIS | Changed this */
                bcdToBits(b),
                bcdToBits(c),
                bcdToBits(d),
                Arith40_index_of_chroma(pb),
                Arith40_index_of_chroma(pr)
        };
        *inNewImage = newPixel;
        (void) uarray2;
}

/*
 *  Name      : yToBits
 *  Purpose   : Maps luma float values to luma int values
 *  Parameters: (float) y       =       The value to convert
 *  Output    : An int representing the scaled luma value
 */
static int bcdToBits(float x)
{
        if (x > maxFloat) {
                return maxInt;
        }
        if (x < (-1 * maxFloat)) {
                return -1 * maxInt;
        }
        int result = (int)(x  * scaleFactor);
        return result;
}

static int aToBits(float a) 
{
        int result = (int)(a * maxAInt);
        return result;
}


/*
 *  Name      : decompress
 *  Purpose   : Turn the DCT float image into a DCT int image
 *  Parameters: (Pnm_ppm) image = Image with Dct_ints as pixels
 *  Output    : (None)
 *  Notes     : Assumes that the Pnm_ppm is in proper format, with the
 *              values being Dct_int structs (no way to check :( );
 *              Will CRE if can not allocate new memory to create the new
 *              trimmed image (if necessary)
 */
static void decompress(Pnm_ppm image)
{
        int width  = image -> width;
        int height = image -> height;

        A2Methods_T methods = uarray2_methods_plain;

        A2Methods_UArray2 pixels      = image -> pixels;
        int               size        = sizeof(struct Dct_float);
        A2Methods_UArray2 newImage    = methods -> new(width, height, size);

        methods -> map_row_major(newImage, dequantize, pixels);
        methods -> free(&pixels);
        
        image -> pixels = newImage;
}

/*
 *  Name      : dequantize
 *  Purpose   : Copy the old image data into the new image data 
 *              going from DCT int to DCT float
 *  Parameters: (int)                col     = The current column to copy
 *              (int)                row     = The current row to copy
 *              (A2Methods_UArray2)  uarray2 = The new array to copy into
 *              (A2Methods_Object *) ptr     = The DCT value in the new array
 *              (void *)             cl      = The DCT int image
 *  Output    : (None)
 *  Notes     : Converts the CIE float to RGB float
 */
static void dequantize(int col, int row, A2Methods_UArray2 uarray2, 
                       A2Methods_Object *ptr, void *cl)
{
        A2Methods_T       methods = uarray2_methods_plain;
        A2Methods_UArray2 pixels  = cl;
        Dct_int           data    = methods -> at(pixels, col, row);

        Dct_float inNewImage = ptr;
        int     a          = data -> a;
        int     b          = data -> b;
        int     c          = data -> c;
        int     d          = data -> d;
        int     pb         = data -> pb;
        int     pr         = data -> pr;
        struct Dct_float newPixel = {
                aToFloat(a),   /*TODO: FIX THIS | Changed this */
                bcdToFloat(b),
                bcdToFloat(c),
                bcdToFloat(d),
                Arith40_chroma_of_index(pb),
                Arith40_chroma_of_index(pr)
        };
        *inNewImage = newPixel;
        (void) uarray2;

}

/*
 *  Name      : bcdToFloat
 *  Purpose   : Maps luma int values to luma float values
 *  Parameters: (unsigned) x       =   The value to convert
 *  Output    : An float representing the scaled luma value
 */
static float bcdToFloat(int x)
{
        if (x > maxInt) {
                return maxFloat;
        }
        if (x < (-1 * maxInt)) { // TODO:  can't happen because unsigned
                return -1 * maxFloat;
        }
        float result = 1.0 * x / scaleFactor;
        return result;
}
static float aToFloat(int a) 
{
        float result = 1.0 * a / maxAInt;
        return result;
}

static struct CompressionStep quantizationStepStruct = {
        compress,
        decompress
};

CompressionStep quantizationStep = &quantizationStepStruct;

#undef numBCDBits 
#undef numABits 
#undef maxFloat

#undef maxInt 
#undef maxAInt 
#undef scaleFactor 