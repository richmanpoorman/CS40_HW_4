#include <pnm.h>
#include <a2plain.h>
#include "CompressionStep.h"
#include <a2methods.h>  
#include "PixelStructs.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

static void compress(Pnm_ppm image);
static void decompress(Pnm_ppm image);

static void cieToDct(int col, int row, A2Methods_UArray2 uarray2, 
                     A2Methods_Object *ptr, void *cl);
static void dctToCie(int col, int row, A2Methods_UArray2 uarray2, 
                     A2Methods_Object *ptr, void *cl);

static float clampToRange(float input, float lower, float upper);

/*
 *  Name      : compress
 *  Purpose   : Turns the pixels from Cie_float to Dct_float
 *  Parameters: (Pnm_ppm) image = Image with Cie_floats as pixels
 *  Output    : (None)
 *  Notes     : Assumes that the Pnm_ppm is in proper format, with the
 *              values being Pnm_rgb structs (no way to check :( );
 *              Will CRE if can not allocate new memory
 */
static void compress(Pnm_ppm image)
{
        assert(image != NULL);
        int width  = image -> width;
        int height = image -> height;

        int newWidth  = width  / 2;
        int newHeight = height / 2;

        A2Methods_T methods = uarray2_methods_plain;
        
        A2Methods_UArray2 pixels   = image -> pixels;
        int               size     = sizeof(struct Dct_float);
        A2Methods_UArray2 newImage = methods -> new(newWidth, newHeight, size);
        
        assert(image    != NULL);
        assert(newImage != NULL);

        methods -> map_row_major(newImage, cieToDct, pixels);
        methods -> free(&pixels);
        
        image -> pixels = newImage;
        image -> width  = newWidth;
        image -> height = newHeight;
}

static void cieToDct(int col, int row, A2Methods_UArray2 uarray2, 
                     A2Methods_Object *ptr, void *cl)
{

        A2Methods_T       methods    = uarray2_methods_plain;
        A2Methods_UArray2 pixels     = cl;
        Dct_float         inNewImage = ptr;

        int oldCol = col * 2;
        int oldRow = row * 2;

        Cie_float topLeft  = methods -> at(pixels, oldCol    , oldRow    );
        Cie_float topRight = methods -> at(pixels, oldCol + 1, oldRow    );
        Cie_float botLeft  = methods -> at(pixels, oldCol    , oldRow + 1);
        Cie_float botRight = methods -> at(pixels, oldCol + 1, oldRow + 1);

        float pbAverage = (
                           topLeft  -> pb +
                           topRight -> pb + 
                           botLeft  -> pb +
                           botRight -> pb
                          ) / 4;
        float prAverage = (
                           topLeft  -> pr +
                           topRight -> pr + 
                           botLeft  -> pr +
                           botRight -> pr
                          ) / 4;
        
        float y1 = topLeft  -> y;
        float y2 = topRight -> y;
        float y3 = botLeft  -> y;
        float y4 = botRight -> y;

        float a = (y4 + y3 + y2 + y1) / 4;
        float b = (y4 + y3 - y2 - y1) / 4;
        float c = (y4 - y3 + y2 - y1) / 4;
        float d = (y4 - y3 - y2 + y1) / 4;

        struct Dct_float newPixel = {
                clampToRange(a, 0.0, 1.0), 
                clampToRange(b, -0.5, 0.5), 
                clampToRange(c, -0.5, 0.5), 
                clampToRange(d, -0.5, 0.5), 
                clampToRange(pbAverage, -0.5, 0.5), 
                clampToRange(prAverage, -0.5, 0.5)
        };

        *inNewImage = newPixel;
        (void) uarray2;
}


/*
 *  Name      : decompress
 *  Purpose   : Turns the pixels from Rgb_float to Pnm_rgb
 *              going from RGB float to RGB int
 *  Parameters: (Pnm_ppm) image = The original image convert format
 *  Output    : (None)
 *  Notes     : Assumes that the Pnm_ppm is in proper format, with the
 *              values being Rgb_float structs (no way to check :( );
 *              Will CRE if can not allocate new memory
 */
static void decompress(Pnm_ppm image)
{
        assert(image != NULL);

        int width  = image -> width;
        int height = image -> height;

        int newWidth  = width  * 2;
        int newHeight = height * 2; 


        A2Methods_T methods = uarray2_methods_plain;

        A2Methods_UArray2 pixels   = image -> pixels;
        int               size     = sizeof(struct Cie_float);
        A2Methods_UArray2 newImage = methods -> new(newWidth, newHeight, size);

        assert(image    != NULL);
        assert(newImage != NULL);

        methods -> map_row_major(newImage, dctToCie, pixels);
        methods -> free(&pixels);
        
        image -> pixels = newImage;
        image -> width  = newWidth;
        image -> height = newHeight;
}

static void dctToCie(int col, int row, A2Methods_UArray2 uarray2, 
                     A2Methods_Object *ptr, void *cl)
{
        A2Methods_T       methods    = uarray2_methods_plain;
        A2Methods_UArray2 pixels     = cl;
        Cie_float         inNewImage = ptr;

        int oldCol = col / 2;
        int oldRow = row / 2;

        

        Dct_float data   = methods -> at(pixels, oldCol, oldRow); 
        int       yIndex = (col % 2) + 2 * (row % 2) + 1;
        
        float a = data -> a;
        float b = data -> b;
        float c = data -> c;
        float d = data -> d;

        float y = -1;
        switch(yIndex) {
                case 1:
                y = a - b - c - d;
                break;
                
                case 2:
                y = a - b + c - d;
                break;

                case 3:
                y = a + b - c - d;
                break;

                case 4: 
                y = a + b + c + d;
                break;

                default:
                assert(false);
        }

        struct Cie_float newPixel = {
                clampToRange(y, 0.0, 1.0),
                clampToRange(data -> pb, -0.5, 0.5), 
                clampToRange(data -> pr, -0.5, 0.5)
        };

        *inNewImage = newPixel;
        (void) uarray2;
}

static float clampToRange(float input, float lower, float upper)
{
        if (input < lower) {
                input = lower;
        }
        if (input > upper) {
                input = upper;
        }
        return input;
}

static struct CompressionStep DCTStruct = {
        compress,
        decompress
};

CompressionStep DCTStep = &DCTStruct;

