#include "arith.h"
#include "a2plain.h"
#include <pnm.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void test(FILE *input, FILE *output)
{
        Pnm_ppm image = Pnm_ppmread(input, uarray2_methods_plain);

        compress40Image(image);
        decompress40Image(image);

        Pnm_ppmwrite(output, image);
        Pnm_ppmfree(&image);
}

int main(int argc, char *argv[])
{

        FILE *input = stdin;
        if (argc >= 2 && strcmp(argv[1], "-") != 0) {
                input = fopen(argv[1], "r");
                assert(input != NULL);
        }
        
        FILE *output = stdout;
        if (argc >= 3 && strcmp(argv[2], "-") != 0) {
                output = fopen(argv[2], "w");
                assert(output != NULL);
        } 

        test(input, output);


        if (input != stdin) {
                fclose(input);
        }
        if (output != stdout) {
                fclose(output);
        }
}