#include <stdio.h>
#include "CompressionStep.h"



void compress40(FILE *input);
void decompress40(FILE *input);

void compress40Output(FILE *input, FILE *output);
void decompress40Output(FILE *input, FILE *output);

/* Steps in order of compression */
CompressionStep ordering[] = {NULL};
size_t          numSteps   = sizeof(ordering) / sizeof(CompressionStep);


void compress40(FILE *input)
{
        compress40Output(input, stdout);
}
void decompress40(FILE *input)
{
        decompress40Output(input, stdout);
}

void compress40Output(FILE *input, FILE *output) 
{
        Pnm_ppm image = Pnm_ppmrdr(input);
        for (size_t step = 0; step < numSteps; step++) {
                ordering[step] -> compress(image);
        }
        Pnm_ppmwrite(output, image);
        Pnm_ppmfree(&image);
}
void decompress40Output(FILE *input, FILE *output)
{
        (void) input;
        (void) output;
}