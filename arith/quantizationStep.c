#include <pnm.h>
#include <a2plain.h>
#include "CompressionStep.h"
#include <a2methods.h>  
#include "PixelStructs.h"
#include <stdio.h>

static void compress(Pnm_ppm image);
static void decompress(Pnm_ppm image);

static void quantize(int col, int row, A2Methods_UArray2 uarray2, 
                     A2Methods_Object *ptr, void *cl);
static void dequantize(int col, int row, A2Methods_UArray2 uarray2, 
                       A2Methods_Object *ptr, void *cl);

static unsigned yToBits(float y);
static float    yToFloat(unsigned y);

/*
 *  Name      : compress
 *  Purpose   : Turn the RGB float image into a CIE float image
 *  Parameters: (Pnm_ppm) image = The original image to trim
 *  Output    : (None)
 *  Notes     : Assumes that the Pnm_ppm is in proper format, with the
 *              values being Rgb_float structs (no way to check :( );
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
static void quantize(int col, int row, A2Methods_UArray2 uarray2, 
                     A2Methods_Object *ptr, void *cl)
{
        
}
static unsigned yToBits(float y)
{

}


/*
 *  Name      : decompress
 *  Purpose   : Turn the CIE float image into a RGB float image
 *  Parameters: (Pnm_ppm) image = The original image to trim
 *  Output    : (None)
 *  Notes     : Assumes that the Pnm_ppm is in proper format, with the
 *              values being Cie_float structs (no way to check :( );
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
static void dequantize(int col, int row, A2Methods_UArray2 uarray2, 
                       A2Methods_Object *ptr, void *cl)
{

}
static float yToFloat(unsigned y)
{

}

static struct CompressionStep quantizationStepStruct = {
        compress,
        decompress
};

CompressionStep quantizationStep = &quantizationStepStruct;
