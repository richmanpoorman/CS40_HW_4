#include "arith.h"
#include "a2plain.h"
#include <pnm.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include "bitpack.h"


void onesCanGoIn() {
        int wordLength = 64;

        for (int wordSize = 1; wordSize <= wordLength; wordSize++) {
                
                /* Make a string of all 1s */
                int remainingSize = wordLength - wordSize;
                uint64_t value = ~0;
                value <<= remainingSize;
                value >>= remainingSize;


                /* Test that all 1s can go in the beginning */
                uint64_t word = 0;
                uint64_t result = 0;
                word = Bitpack_newu(word, wordSize, 0, value);
                result = Bitpack_getu(word, wordSize, 0);
                assert(result == value);

                /* Test that all 1s can go in the end */
                word = 0;
                word = Bitpack_newu(word, wordSize, remainingSize, value);
                result = Bitpack_getu(word, wordSize, remainingSize);
                assert(result == value);

                /* Test that all 1s can go in the middle */
                word = 0;
                word = Bitpack_newu(word, wordSize, remainingSize / 2, value);
                result = Bitpack_getu(word, wordSize, remainingSize / 2);
                fprintf(stderr, "Value: %lu, Result: %lu\n", value, result);
                assert(result == value);

        }
}

void test(FILE *input, FILE *output)
{
        

        (void) input;
        (void) output;
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