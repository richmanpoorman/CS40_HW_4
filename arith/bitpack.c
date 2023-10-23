#include <stdbool.h>
#include <stdint.h>

#include <assert.h>
#include <except.h>


#define MAX_WIDTH 64

bool Bitpack_fitsu(uint64_t n, unsigned width);
bool Bitpack_fitss( int64_t n, unsigned width);
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb);
 int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb);
uint64_t Bitpack_newu(uint64_t word, unsigned width, 
                      unsigned lsb, uint64_t value);
uint64_t Bitpack_news(uint64_t word, unsigned width, 
                      unsigned lsb,  int64_t value);

/* Implementation */
bool Bitpack_fitsu(uint64_t n, unsigned width) 
{
        assert(width <= MAX_WIDTH);
        if (width == MAX_WIDTH) {
                return true;
        }
        uint64_t maxValue = 1;
        maxValue <<= width;
        return n < maxValue;
}
bool Bitpack_fitss( int64_t n, unsigned width) 
{
        assert(width <= MAX_WIDTH);
        /* Can fit all data when we have the max amount of bits */
        if (width == MAX_WIDTH) {
                return true;
        }

        /* Can't fit anything because of the signed bit */
        if (width <= 1) {
                return false;
        }
        /* The compliment takes the same amount of space for bits */
        if (n < 0) { 
                n = ~n;
        }
        /* Saves 1 bit for the sign, then checks if the value can fit */
        return Bitpack_fitsu(n, width - 1);
}
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        
        assert(width <= MAX_WIDTH);
        assert(lsb <= MAX_WIDTH);
        assert(width + lsb <= MAX_WIDTH);

        /* If the data gotten is empty */
        if (width == 0) {
                return 0;
        }

        /* Get a line of all 1s */
        uint64_t mask = 0;
        mask = ~mask;
        
        /* 
         *  Only want (width) amount of 1s, so get rid of 
         *  MAX_WIDTH - (width) amount of 1s (as we start with MAX_WIDTH 1s)
         */
        mask >>= MAX_WIDTH - width;

        /* 
         *  We need the mask to align with the position in the word, which
         *  is (lsb) bits over to the left
         */
        mask <<= lsb;

        /* Retrieve the data from the word */
        uint64_t maskGet = word & mask;

        /* We have the data, but it is shifted (lsb) over, so we unshift */
        maskGet >>= lsb;
        return maskGet;

}

#include <stdio.h>
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= MAX_WIDTH);
        assert(lsb <= MAX_WIDTH);
        assert(width + lsb <= MAX_WIDTH);
        
        /* If the data gotten is empty (including the signed bit) */
        if (width <= 1) {
                return 0;
        }

        /* 
         *  The cast here stays within range, as since it is taking
         *  width - 1, it will fit into a int64_t
         */
        int64_t  value = (int64_t)Bitpack_getu(word, width, lsb);
        uint64_t sign  = Bitpack_getu(word, 1, lsb + width - 1);
        if (sign > 0) {
                /*
                 *  If it is in the form of 2's complement,
                 *  then instead we start with all 1's and 
                 *  remove the 1's where the value is 0 as bits
                 */
                int64_t zero = 0;
                zero = ~zero;
                /* Move the 1s bits over to copy in the value */
                zero <<= width;
                /* Make the values afterward 1s */
                value = value | zero;
        }
        return value;
}
uint64_t Bitpack_newu(uint64_t word, unsigned width, 
                      unsigned lsb, uint64_t value)
{
        assert(width <= MAX_WIDTH);
        assert(lsb <= MAX_WIDTH);
        assert(width + lsb <= MAX_WIDTH);

        uint64_t mask = 0;
        mask = ~mask;
        
        /* 
         *  Only want (width) amount of 1s, so get rid of 
         *  MAX_WIDTH - (width) amount of 1s (as we start with MAX_WIDTH 1s)
         */
        mask >>= MAX_WIDTH - width;

        uint64_t clearMask = mask;
        /* 
         *  We need the mask to align with the position in the word, which
         *  is (lsb) bits over to the left
         */
        clearMask <<= lsb;

        /* 
         * Want to clear out the 1s, so we can use the compliment 
         * to make all 1s except where we are putting it in
         */
        clearMask = ~clearMask;
        word = word & clearMask;

        /* 
         * Now, to put it in, we first put the value into the mask
         * Note that this is just a check to chop off values that 
         * go out of bounds
        */
        mask = mask & value;

        /* 
         *  We need the mask to align with the position in the word, which
         *  is (lsb) bits over to the left
         */
        mask <<= lsb;

        /* 
         *  Then, since it is cleared out by the steps above, 
         *  we can use | to get the values in, as it will be 
         *  1s if mask is 1, or the value if the mask is 0 
         *  which is 0 in the area we want to put the word in
         */
        word = word | mask;

        return word;
}
uint64_t Bitpack_news(uint64_t word, unsigned width, 
                      unsigned lsb,  int64_t value)
{       
        assert(width <= MAX_WIDTH);
        assert(lsb <= MAX_WIDTH);
        assert(width + lsb <= MAX_WIDTH);
        
        /* 
         * If value has nothing in it, then just don't put it in 
         * Note that it is 1 because then we only have space for the sign
         * bit at 1 width
         */
        if (width <= 1) {
                return word;
        }
        /* 
         * If it takes up the whole word, then just return it
         * Note that we do it this way, as the value doesn't fit into
         * uint64_t otherwise
         */
        if (width == MAX_WIDTH) {
                return value;
        }

        /* Get rid of the excess 1s from 2's compliment */
        uint64_t unsignedValue = (uint64_t)value;
        unsignedValue <<= MAX_WIDTH - width;
        unsignedValue >>= MAX_WIDTH - width;

        return Bitpack_newu(word, width, lsb, unsignedValue);
}

#undef MAX_WIDTH