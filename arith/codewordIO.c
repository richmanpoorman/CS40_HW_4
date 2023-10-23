#include <stdio.h>
#include <pnm.h>
#include "PixelStructs.h"
#include <assert.h>
#include "a2plain.h"
#include "a2methods.h"
#include "codewordInfo.h"
#include "bitpack.h"
#include "mem.h"

Pnm_ppm readCodewordFile(FILE *input);
void writeCodewordFile(Pnm_ppm codewordImage, FILE *output);

static void readData(int col, int row, A2Methods_UArray2 uarray2, 
                      A2Methods_Object *ptr, void *cl);
static void writeData(int col, int row, A2Methods_UArray2 uarray2, 
                      A2Methods_Object *ptr, void *cl);

Pnm_ppm readCodewordFile(FILE *input)
{
        unsigned height, width;
        int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u", 
                          &width, &height);
        assert(read == 2);
        int c = getc(input);
        assert(c == '\n');

        A2Methods_T       methods = uarray2_methods_plain;
        A2Methods_UArray2 pixels  = methods -> new(width, height, 
                                                   sizeof(Codeword));
        
        methods -> map_row_major(pixels, readData, input);

        struct Pnm_ppm imageData = {
                .width       = width,
                .height      = height,
                .denominator = 1,
                .pixels      = pixels,
                .methods     = methods
        };

        Pnm_ppm image = ALLOC(sizeof(imageData));
        *image = imageData;

        return image;
}
static void readData(int col, int row, A2Methods_UArray2 uarray2, 
                      A2Methods_Object *ptr, void *cl)
{
        FILE        *input    = cl;
        Codeword     codeword = ptr;
        
        assert(uarray2 != NULL);
        assert(input != NULL);
        assert(codeword != NULL);

        uint32_t word     = 0;
        int      numBytes = getNumBytes();
        int      byteSize = sizeof(char);

        for (int byte = 0; byte < numBytes; byte++) {
                assert(feof(input) == 0);
                assert(ferror(input) == 0);
                
                char currByte = getc(input);
                assert(ferror(input) == 0);

                Bitpack_newu(word, byteSize * 8, byte * byteSize * 8, 
                             (uint64_t)currByte);
        }
        assert(ferror(input) == 0);

        struct Codeword newPixel = {
                (uint32_t)word
        };

        *codeword = newPixel;
        (void) col;
        (void) row;
        (void) uarray2;

}

void writeCodewordFile(Pnm_ppm codewordImage, FILE *output)
{
        unsigned int width  = codewordImage -> width;
        unsigned int height = codewordImage -> height; 
        fprintf(output, "COMP40 Compressed image format 2\n%u %u\n", 
                width, height);

        A2Methods_T       methods = uarray2_methods_plain;
        A2Methods_UArray2 image   = codewordImage -> pixels;

        methods -> map_row_major(image, writeData, output);
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
static void writeData(int col, int row, A2Methods_UArray2 uarray2, 
                      A2Methods_Object *ptr, void *cl)
{
        
        FILE        *output   = cl;
        Codeword     codeword = ptr;
        
        assert(uarray2 != NULL);
        assert(output != NULL);
        assert(codeword != NULL);

        uint64_t word     = codeword -> codeword;
        int      numBytes = getNumBytes();
        int      byteSize = sizeof(char);

        for (int byte = 0; byte < numBytes; byte++) {
                
                char currByte = (char)Bitpack_getu(word, byteSize * 8, 
                                                   byte * byteSize * 8);
                fprintf(output, "%c", currByte);
        }

        (void) col;
        (void) row;
        (void) uarray2;

}