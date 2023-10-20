#include <stdio.h>
#include "arith.h"

/*
 *  Name      : compress40
 *  Purpose   : Take the image and print the compressed output to stdout
 *  Parameters: (FILE *) input = The input file to read the image from
 *  Output    : (None)
 *  Notes     : Assumes that the file is open
 */
void compress40(FILE *input)
{
        compress40Output(input, stdout);
}

/*
 *  Name      : decompress40
 *  Purpose   : Take the compressed image and prints the 
 *              decompressed image to stdout
 *  Parameters: (FILE *) input = The input file to read the image from
 *  Output    : (None)
 *  Notes     : Assumes that the file is open
 */
void decompress40(FILE *input)
{
        decompress40Output(input, stdout);
}