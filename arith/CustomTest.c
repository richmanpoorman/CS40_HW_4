#include "arith.h"
#include "a2plain.h"
#include <pnm.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
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
                //fprintf(stderr, "Value: %lu, Result: %lu\n", value, result);
                assert(result == value);            
        }
}

void fitsuTests() {
                bool fitsu_lowEdge_succeed = Bitpack_fitsu(0,3);
                assert (fitsu_lowEdge_succeed == true);
                
                bool fitsu_middle_succeed = Bitpack_fitsu(5,3);
                assert (fitsu_middle_succeed == true);
               
                bool fitsu_highEdge_succeed = Bitpack_fitsu(7,3);
                assert (fitsu_highEdge_succeed == true);
               
                bool fitsu_lowEdge_fail = Bitpack_fitsu(-1,3);
                assert (fitsu_lowEdge_fail == false);

                bool fitsu_highEdge_fail = Bitpack_fitsu(8,3);
                assert (fitsu_highEdge_fail == false);
}

void getTests() {
                unsigned getu_test = Bitpack_getu(0x3f4, 6, 2);
                //fprintf(stderr, "getu_test result: %u\n", getu_test);

                signed gets_test = Bitpack_gets(0x3f4, 6, 2);
                //fprintf(stderr, "gets_test result: %i\n", gets_test);

                assert(getu_test == 61);
                assert(gets_test == -3);
}

void widthOfOne() {
                signed gets_negativeOne = Bitpack_gets(0xfff, 1, 1);
                fprintf(stderr, "gets_test result: %i\n", gets_negativeOne);
                assert(gets_negativeOne == -1);
                fprintf(stderr, "this is a -1! :)");
}

void test(FILE *input, FILE *output)
{
        onesCanGoIn();
        fitsuTests();
        getTests();
        widthOfOne();
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