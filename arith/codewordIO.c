#include <stdio.h>
#include <pnm.h>

Pnm_ppm readCodewordFile(FILE *input);
void writeCodewordFile(Pnm_ppm codewordImage, FILE *output);


Pnm_ppm readCodewordFile(FILE *input)
{

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
        A2Methods_T  methods  = uarray2_methods_plain;
        FILE        *output   = cl;
        Codeword     codeword = ptr;

        *inNewImage = newPixel;
        (void) uarray2;

}