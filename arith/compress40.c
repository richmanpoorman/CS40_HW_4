#include <stdio.h>
#include <pnm.h>
#include <a2methods.h>
#include <a2plain.h>
#include <assert.h>

#include "CompressionStep.h"

#include "trimStep.h"
#include "rgbIntToFloatStep.h"
#include "rgbToCieStep.h"


void compress40(FILE *input);
void decompress40(FILE *input);

void compress40Output(FILE *input, FILE *output);
void decompress40Output(FILE *input, FILE *output);

void compress40Image(Pnm_ppm image);
void decompress40Image(Pnm_ppm image);

/* Steps in order of compression */
#define stepOrder {trimStep, rgbIntToFloatStep, rgbToCieStep}

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

/*
 *  Name      : compress40Output
 *  Purpose   : Take the image and print the compressed output to output file
 *  Parameters: (FILE *) input  = The input file to read the image from
 *              (FILE *) output = The output file to print the image to
 *  Output    : (None)
 *  Notes     : Assumes that the file is open
 */
void compress40Output(FILE *input, FILE *output) 
{
        assert(input != NULL && output != NULL);
        Pnm_ppm image = Pnm_ppmread(input, uarray2_methods_plain);
        compress40Image(image);
        // TODO: use the compressed writer
        Pnm_ppmfree(&image);
}

/*
 *  Name      : decompress40Output
 *  Purpose   : Take the compressed image and print the 
 *              decompressed output to output file
 *  Parameters: (FILE *) input  = The input file to read the image from
 *              (FILE *) output = The output file to print the image to
 *  Output    : (None)
 *  Notes     : Assumes that the file is open
 */
void decompress40Output(FILE *input, FILE *output)
{
        assert(input != NULL && output != NULL);
        // TODO: use the compressed reader
        (void) input;
        (void) output;
}

/*
 *  Name      : compress40Image
 *  Purpose   : Takes an image and compresses it in-place
 *  Parameters: (Pnm_ppm) image = The image to compress
 *  Output    : (None)
 *  Notes     : Assumes that the Pnm_ppm is valid
 */
void compress40Image(Pnm_ppm image)
{
        CompressionStep ordering[] = stepOrder;
        int numSteps = sizeof(ordering) / sizeof(CompressionStep);
        for (int step = 0; step < numSteps; step++) {
                ordering[step] -> compress(image);
        }
}

/*
 *  Name      : decompress40Image
 *  Purpose   : Takes an image and decompresses it in-place
 *  Parameters: (Pnm_ppm) image = The image to decompress
 *  Output    : (None)
 *  Notes     : Assumes that the Pnm_ppm is valid
 */
void decompress40Image(Pnm_ppm image)
{
        CompressionStep ordering[] = stepOrder;
        int numSteps = sizeof(ordering) / sizeof(CompressionStep);
        for (int step = numSteps - 1; step >= 0; step--) {
                ordering[step] -> decompress(image);
        }
}

#undef ordering 