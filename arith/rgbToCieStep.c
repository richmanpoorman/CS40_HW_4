#include <pnm.h>
#include <a2plain.h>
#include "CompressionStep.h"
#include <a2methods.h>  
#include "PixelStructs.h"
#include <stdio.h>

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
        int               size        = sizeof(struct Cie_float);
        A2Methods_UArray2 newImage    = methods -> new(width, height, size);
        
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
        Rgb_float         data    = methods -> at(pixels, col, row);

        Cie_float inNewImage = ptr;
        float     r          = data -> red;
        float     g          = data -> green;
        float     b          = data -> blue;
        struct Cie_float newPixel = {
                rToY  * r + gToY  * g + bToY  * b,
                rToPb * r + gToPb * g + bToPb * b,
                rToPr * r + gToPr * g + bToPr * b
        };
        *inNewImage = newPixel;
        (void) uarray2;
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
        int               size        = sizeof(struct Rgb_float);
        A2Methods_UArray2 newImage    = methods -> new(width, height, size);

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
        Cie_float         data    = methods -> at(pixels, col, row);

        Rgb_float inNewImage = ptr;
        float     y          = data -> y;
        float     pb         = data -> pb;
        float     pr         = data -> pr;
        struct Rgb_float newPixel = {
                yToR * y + pbToR * pb + prToR * pr,
                yToG * y + pbToG * pb + prToG * pr,
                yToB * y + pbToB * pb + prToB * pr
        };
        *inNewImage = newPixel;
        (void) uarray2;
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