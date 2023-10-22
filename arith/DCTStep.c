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

/*
 *  Name      : compress
 *  Purpose   : Turns the pixels from Cie_float to Dct_float
 *  Parameters: (Pnm_ppm) image = Image with Cie_floats as pixels
 *  Output    : (None)
 *  Notes     : Assumes that the Pnm_ppm is in proper format, with the
 *              values being Pnm_rgb structs (no way to check :( );
 *              Will CRE if can not allocate new memory;
 *              Will change the width and height of the image
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

/*
 *  Name      : cieToDct
 *  Purpose   : Put the correct DCT pixel into the image for each pixel
 *  Parameters: (int)                col     = The column of the current pixel
 *              (int)                row     = The row of the current pixel
 *              (A2Methods_UArray2)  uarray2 = The new image pixels
 *              (A2Methods_Object *) ptr     = The DCT pixel in the new image
 *              (void *)             cl      = The CIE image to convert from
 *  Output    : (None)
 *  Notes     : Assumes that the Pnm_ppm is in proper format, with the
 *              values being Pnm_rgb structs (no way to check :( );
 *              Should only be used as a mapping function to the new 
 *              DCT pixel array
 */
static void cieToDct(int col, int row, A2Methods_UArray2 uarray2, 
                     A2Methods_Object *ptr, void *cl)
{

        A2Methods_T       methods    = uarray2_methods_plain;
        A2Methods_UArray2 pixels     = cl;
        Dct_float         inNewImage = ptr;

        int oldCol = col * 2;
        int oldRow = row * 2;

        /* Gets the 2x2 pixels values */
        Cie_float topLeft  = methods -> at(pixels, oldCol    , oldRow    );
        Cie_float topRight = methods -> at(pixels, oldCol + 1, oldRow    );
        Cie_float botLeft  = methods -> at(pixels, oldCol    , oldRow + 1);
        Cie_float botRight = methods -> at(pixels, oldCol + 1, oldRow + 1);

        /* Gets the average of the 2x2 block for pb and pr */
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
        
        /* Gets Y1, Y2, Y3, Y4 */
        float y1 = topLeft  -> y;
        float y2 = topRight -> y;
        float y3 = botLeft  -> y;
        float y4 = botRight -> y;

        /* Converts y -> a, b, c, d for the dct format*/
        float a = (y4 + y3 + y2 + y1) / 4.0;
        float b = (y4 + y3 - y2 - y1) / 4.0;
        float c = (y4 - y3 + y2 - y1) / 4.0;
        float d = (y4 - y3 - y2 + y1) / 4.0;

        /* Puts the DCT pixel on the stack */
        struct Dct_float newPixel = {
                clampToRange(a        ,  0.0, 1.0), 
                clampToRange(b        , -0.5, 0.5), 
                clampToRange(c        , -0.5, 0.5), 
                clampToRange(d        , -0.5, 0.5), 
                clampToRange(pbAverage, -0.5, 0.5), 
                clampToRange(prAverage, -0.5, 0.5)
        };

        *inNewImage = newPixel;
        (void) uarray2;
}


/*
 *  Name      : decompress
 *  Purpose   : Turns the pixels from Dct_float to Cie_float
 *              going from DCT float to CIE int
 *  Parameters: (Pnm_ppm) image = Image with DCT floats as pixels
 *  Output    : (None)
 *  Notes     : Assumes that the Pnm_ppm is in proper format, with the
 *              values being Rgb_float structs (no way to check :( );
 *              Will CRE if can not allocate new memory;
 *              Will change the width and height of the image
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

/*
 *  Name      : dctToCie
 *  Purpose   : Put the correct CIE pixel into the image for each pixel
 *  Parameters: (int)                col     = The column of the current pixel
 *              (int)                row     = The row of the current pixel
 *              (A2Methods_UArray2)  uarray2 = The new image pixels
 *              (A2Methods_Object *) ptr     = The CIE pixel in the new image
 *              (void *)             cl      = The DCT image to convert from
 *  Output    : (None)
 *  Notes     : Assumes that the Pnm_ppm is in proper format, with the
 *              values being Pnm_rgb structs (no way to check :( );
 *              Should only be used as a mapping function to the new 
 *              CIE pixel array
 */
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
                y = a - b - c + d;
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
                clampToRange(y         ,  0.0, 1.0),
                clampToRange(data -> pb, -0.5, 0.5), 
                clampToRange(data -> pr, -0.5, 0.5)
        };

        *inNewImage = newPixel;
        (void) uarray2;
}


static struct CompressionStep DCTStruct = {
        compress,
        decompress
};

CompressionStep DCTStep = &DCTStruct;

