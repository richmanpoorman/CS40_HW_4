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


void onesCanGoIn() 
{
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
                /* fprintf(stderr, "Value: %lu, Result: %lu\n", value, result);
                */
                assert(result == value);            
        }
}

void fitsuTests() 
{
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

void getTests() 
{
        unsigned getu_test = Bitpack_getu(0x3f4, 6, 2);
        /* fprintf(stderr, "getu_test result: %u\n", getu_test); */

        signed gets_test = Bitpack_gets(0x3f4, 6, 2);
        /* fprintf(stderr, "gets_test result: %i\n", gets_test); */
        assert(getu_test == 61);
        assert(gets_test == -3);
}

void widthOfOne() {
        signed gets_negativeOne = Bitpack_gets(0xfff, 1, 1);
        fprintf(stderr, "here is a -1: %i\n", gets_negativeOne);
        assert(gets_negativeOne == -1);

        signed gets_zero = Bitpack_gets(0x000, 1, 1);
        fprintf(stderr, "here is a 0: %i\n", gets_zero);
        assert(gets_zero == 0);

}

void sanityCheck() 
{
        uint64_t word = 0x3f4;
        unsigned w = 6;
        unsigned w2 = 8;
        unsigned lsb = 2;
        uint64_t val = 3;
        unsigned lsb2 = 9;       
        assert(Bitpack_getu(Bitpack_newu(word, w, lsb, val), w, lsb) == val);
        assert(Bitpack_getu(Bitpack_newu(word, w, lsb, val), w2, lsb2) == 
                Bitpack_getu(word, w2, lsb2));
        /* printf("this is ok\n"); */
}

void tryExceptTest() 
{
        uint64_t word = 0x3f4;
        unsigned w = 2;
        unsigned lsb = 2;
        uint64_t val = 8; 
        TRY
                Bitpack_newu(word, w, lsb, val);
        EXCEPT(Bitpack_Overflow);
                printf("GOOD JOB\n");
        END_TRY;

        uint64_t val2 = 8; 
        TRY
                Bitpack_news(word, w, lsb, val2);
        EXCEPT(Bitpack_Overflow);
                printf("GOOD JOB AGAIN\n");
        END_TRY;

}

void maxSignedTest()
{
        int64_t allOnes = -1;
        uint64_t word = 0;
        uint64_t fitSmall = Bitpack_news(word, 4, 10, allOnes);
        assert(-1 == Bitpack_gets(fitSmall, 4, 10));

        uint64_t fitBig = Bitpack_news(word, 64, 0, allOnes);
        assert(-1 == Bitpack_gets(fitBig, 64, 0));

        uint64_t fitZero = Bitpack_news((uint64_t)allOnes, 0, 4, 0);
        assert(0 == Bitpack_gets(fitZero, 0, 4));

        int64_t bitValue = 0x7af9fc12faf9fc12;
        uint64_t random64Bit = Bitpack_news(word, 64, 0, bitValue);
        assert(bitValue == Bitpack_gets(random64Bit, 64, 0));

        uint64_t oneBit = Bitpack_news(word, 1, 31, -1);
        uint64_t oneBit2 = Bitpack_news(word, 1, 63, 0);
        assert(-1 == Bitpack_gets(oneBit, 1, 31));
        assert(0 == Bitpack_gets(oneBit2, 1, 63));

        uint64_t maxValue = Bitpack_news(word, 5, 14, 15);
        uint64_t minValue = Bitpack_news(maxValue, 5, 18, -16);
        assert(15 == Bitpack_gets(minValue, 5, 14));
        assert(-16 == Bitpack_gets(minValue, 5, 18));

        int64_t bigNumber = (int64_t)0xC000000000000000;
        uint64_t almostFull = Bitpack_news(word, 63, 1, bigNumber);
        assert(bigNumber == Bitpack_gets(almostFull, 63, 1));
        

        uint64_t emptyBits = Bitpack_news((uint64_t)allOnes, 14, 46, -12);
        assert(-12 == Bitpack_gets(emptyBits, 14, 46));

        uint64_t putSigned   = Bitpack_news(word, 5, 0, -10);
        uint64_t putUnsigned = Bitpack_news(putSigned, 5, 5, 6);
        assert(-10 == Bitpack_gets(putUnsigned, 5, 0));
        assert(6 == Bitpack_gets(putUnsigned, 5, 5));
        fprintf(stderr, "Max Signed Test Done\n");
}

void test(FILE *input, FILE *output)
{
        onesCanGoIn();
        fitsuTests();
        getTests();
        widthOfOne();
        sanityCheck();
        tryExceptTest();
        maxSignedTest();
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