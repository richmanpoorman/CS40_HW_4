/* 
 *   Name       : codewordIO.c
 *   Assignment : CS40 Homework 4 (arith)
 *   Purpose    : Module for reading and writing codewords
 *   Editors    : Matthew Wong (mwong14), Ivi Fung (sfung02)
 */

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

/*
 *  Name      : readCodewordFile
 *  Purpose   : Reads a file containing codewords for a compressed image
 *  Parameters: (FILE *)  input = The file to read
 *  Output    : (Pnm_ppm) Returns the image in the Codeword format
 */
Pnm_ppm readCodewordFile(FILE *input)
{
        assert(input != NULL);
        unsigned height, width;
        int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u", 
                          &width, &height);
        assert(read == 2);
        int c = getc(input);
        assert(c == '\n');

        width  /= 2;
        height /= 2;
        int size = sizeof(struct Codeword);
        A2Methods_T       methods = uarray2_methods_plain;
        A2Methods_UArray2 pixels  = methods -> new(width, height, size);

        assert(pixels != NULL);

        methods -> map_row_major(pixels, readData, input);

        Pnm_ppm image = ALLOC(sizeof(*image));
        image -> width       = width;
        image -> height      = height;
        image -> denominator = 1;
        image -> pixels      = pixels;
        image -> methods     = methods;

        return image;
}

/*
 *  Name      : readData
 *  Purpose   : Reads image data one codeword at a time
 *  Parameters: (int)                col     = The current column to copy
 *              (int)                row     = The current row to copy
 *              (A2Methods_UArray2)  uarray2 = The new array to copy into
 *              (A2Methods_Object *) ptr     = The codeword to read
 *              (void *)             cl      = The input file
 *  Output    : (None)
 *  Notes     : Prints a codeword to the given file in big endian order
 */
static void readData(int col, int row, A2Methods_UArray2 uarray2, 
                      A2Methods_Object *ptr, void *cl)
{
        FILE        *input    = cl;
        Codeword     codeword = ptr;
        
        assert(uarray2 != NULL);
        assert(input != NULL);
        assert(codeword != NULL);

        uint64_t word     = 0;
        int      numBytes = getNumBytes();
        int      byteSize = sizeof(char);

        /* Reversed to do big-endian order */
        for (int byte = numBytes - 1; byte >= 0; byte--) {
                assert(feof(input) == 0);
                assert(ferror(input) == 0);
                
                unsigned int currByte = getc(input);
                assert(ferror(input) == 0);

                word = Bitpack_newu(word, byteSize * 8, byte * byteSize * 8, 
                                    currByte);
        }
        
        assert(ferror(input) == 0);
        struct Codeword newPixel = {
                word
        };


        *codeword = newPixel;
        (void) col;
        (void) row;
        (void) uarray2;

}

/*
 *  Name      : writeCodewordFile
 *  Purpose   : Writes a file containing codewords for a compressed image
 *  Parameters: (Pnm_ppm) codewordImage = The compressed image to write out
 *              (FILE *)  output        = The file to write to
 *  Output    : (None)
 *  Notes     : Writes a file containing codewords in big endian format
 */
void writeCodewordFile(Pnm_ppm codewordImage, FILE *output)
{
        assert(codewordImage != NULL);
        assert(output != NULL);
        unsigned int width  = codewordImage -> width * 2;
        unsigned int height = codewordImage -> height * 2; 
        fprintf(output, "COMP40 Compressed image format 2\n%u %u\n", 
                width, height);

        A2Methods_T       methods = uarray2_methods_plain;
        A2Methods_UArray2 pixels  = codewordImage -> pixels;

        assert(pixels != NULL);

        methods -> map_row_major(pixels, writeData, output);
}

/*
 *  Name      : writeData
 *  Purpose   : Copy the old image data into the new image data 
 *              one codeword at a time
 *  Parameters: (int)                col     = The current column to copy
 *              (int)                row     = The current row to copy
 *              (A2Methods_UArray2)  uarray2 = The new array to copy into
 *              (A2Methods_Object *) ptr     = The codeword to write
 *              (void *)             cl      = The output file
 *  Output    : (None)
 *  Notes     : Prints a codeword to the given file in big endian order
 */
static void writeData(int col, int row, A2Methods_UArray2 uarray2, 
                      A2Methods_Object *ptr, void *cl)
{
        
        FILE     *output   = cl;
        Codeword  codeword = ptr;
        
        assert(uarray2 != NULL);
        assert(output != NULL);
        assert(codeword != NULL);

        uint64_t word     = codeword -> codeword;
        int      numBytes = getNumBytes();
        int      byteSize = sizeof(char);

        /* Reversed to do big-endian order */
        for (int byte = numBytes - 1; byte >= 0; byte--) {
                
                unsigned int currByte = (unsigned int)Bitpack_getu(
                                                word, byteSize * 8, 
                                                byte * byteSize * 8);
                fprintf(output, "%c", currByte);
        }

        (void) col;
        (void) row;
        (void) uarray2;

}