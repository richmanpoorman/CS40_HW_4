#include <stdbool.h>
#include <stdint.h>

#include <assert.h>
#include <except.h>


#define MAX_WIDTH 64

Except_T Bitpack_Overflow = { "Overflow packing bits" };

bool Bitpack_fitsu(uint64_t n, unsigned width);
bool Bitpack_fitss( int64_t n, unsigned width);
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb);
 int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb);
uint64_t Bitpack_newu(uint64_t word, unsigned width, 
                      unsigned lsb, uint64_t value);
uint64_t Bitpack_news(uint64_t word, unsigned width, 
                      unsigned lsb,  int64_t value);
static uint64_t leftShift(uint64_t word, unsigned count);
static uint64_t rightShift(uint64_t word, unsigned count);
static uint64_t complement(uint64_t word);

/* Implementation */

/*
 *  Name      : Bitpack_fitsu
 *  Purpose   : Checks if the unsigned value can be packed into (width) bits
 *  Parameters: (uint64_t) n     = The value to check if it fits
 *              (unsigned) width = The number of bits that it can check against
 *  Output    : (bool) True if the number can fit in width bits, false 
 *                     if it can not
 *  Notes     : Will CRE if given a width greater than 64
 *              Will let 0 fit into 0 bits
 */
bool Bitpack_fitsu(uint64_t n, unsigned width) 
{
        assert(width <= MAX_WIDTH);

        /* All 64 bit numbers can fit in 64 bits */
        if (width == MAX_WIDTH) {
                return true;
        }
        /* 
         *  Otherwise, find the upper bound of 2^(width) - 1, which 
         *  means that we can check if it is strictly less than 2^(width)
         */

        uint64_t maxValue = leftShift(1, width);
        return n < maxValue;
}

/*
 *  Name      : Bitpack_fitss
 *  Purpose   : Checks if the signed value can be packed into (width) bits
 *  Parameters: (int64_t)  n     = The value to check if it fits
 *              (unsigned) width = The number of bits that it can check against
 *  Output    : (bool) True if the number can fit in width bits, false 
 *                     if it can not
 *  Notes     : Will CRE if given a width greater than 64
 *              Will not let anything fit in 0 bits, but will fit
 *                      [-1, 0] in 1 bit
 */
bool Bitpack_fitss( int64_t n, unsigned width) 
{
        assert(width <= MAX_WIDTH);
        /* Can fit all data when we have the max amount of bits */
        if (width == MAX_WIDTH) {
                return true;
        }

        /* Can't fit anything in 0-bits */
        if (width < 1) {
                return false;
        }
        /* The compliment takes the same amount of space for bits */
        if (n < 0) { 
                n = (int64_t)complement((uint64_t)n);
        }
        /* Saves 1 bit for the sign, then checks if the value can fit */
        return Bitpack_fitsu((uint64_t)n, width - 1);
}

/*
 *  Name      : Bitpack_getu
 *  Purpose   : Get the packed value of size (width) at (lsb) in (word)
 *              for unsigned values
 *  Parameters: (uint64_t) word  = The word with the packed data
 *              (unsigned) width = The number of bits of the value in (word)
 *              (unsigned) lsb   = The least significant bit of the value 
 *                                 to search for in (word)
 *  Output    : (uint64_t) The value stored in the word
 *  Notes     : Will CRE if given a width greater than 64
 *              Will CRE if lsb is greater than 64
 *              Will CRE if width + lsb is greater than 64
 *              Will return 0 if given a width of 0
 */
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
        uint64_t mask = complement(0);
        
        /* 
         *  Only want (width) amount of 1s, so get rid of 
         *  MAX_WIDTH - (width) amount of 1s (as we start with MAX_WIDTH 1s)
         */
        mask = rightShift(mask, MAX_WIDTH - width);

        /* 
         *  We need the mask to align with the position in the word, which
         *  is (lsb) bits over to the left
         */
        mask = leftShift(mask, lsb);

        /* Retrieve the data from the word */
        uint64_t maskGet = word & mask;

        /* We have the data, but it is shifted (lsb) over, so we unshift */
        maskGet = rightShift(maskGet, lsb);
        return maskGet;

}

/*
 *  Name      : Bitpack_gets
 *  Purpose   : Get the packed value of size (width) at (lsb) in (word) 
 *              for signed values
 *  Parameters: (uint64_t) word  = The word with the packed data
 *              (unsigned) width = The number of bits of the value in (word)
 *              (unsigned) lsb   = The least significant bit of the value 
 *                                 to search for in (word)
 *  Output    : (int64_t) The value stored in the word
 *  Notes     : Will CRE if given a width greater than 64
 *              Will CRE if lsb is greater than 64
 *              Will CRE if width + lsb is greater than 64
 *              Will return 0 if given a width of 0
 */
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= MAX_WIDTH);
        assert(lsb <= MAX_WIDTH);
        assert(width + lsb <= MAX_WIDTH);
        
        /* If the data gotten is empty */
        if (width == 0) {
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
                int64_t allOnes = (int64_t)complement(0);
                /* Move the 1s bits over to copy in the value */
                allOnes = (int64_t)leftShift((uint64_t)allOnes, width);
                /* Make the values afterward 1s */
                value = value | allOnes;
        }
        return value;
}

/*
 *  Name      : Bitpack_newu
 *  Purpose   : Pack (value) using (width) bits at (lsb) into (word) for 
 *              unsigned values
 *  Parameters: (uint64_t) word  = The word to pack into
 *              (unsigned) width = The number of bits that (value) will use
 *              (unsigned) lsb   = The position of the least significant bit 
 *                                 of (value) in (word)
 *              (uint64_t) value = The value to be packed into the word
 *  Output    : (uint64_t) The word after packing value in
 *  Notes     : Will CRE if given a width greater than 64
 *              Will CRE if lsb is greater than 64
 *              Will CRE if width + lsb is greater than 64
 *              Will fully replace the word at width equal to 64
 *              Will return the word for width equal to 0
 */
uint64_t Bitpack_newu(uint64_t word, unsigned width, 
                      unsigned lsb, uint64_t value)
{
        assert(width <= MAX_WIDTH);
        assert(lsb <= MAX_WIDTH);
        assert(width + lsb <= MAX_WIDTH);

        if (width == MAX_WIDTH) {
                return value;
        }
        if (width == 0) {
                return word;
        }

        uint64_t mask = complement(0);
        
        /* 
         *  Only want (width) amount of 1s, so get rid of 
         *  MAX_WIDTH - (width) amount of 1s (as we start with MAX_WIDTH 1s)
         */
        mask = rightShift(mask, MAX_WIDTH - width);

        uint64_t clearMask = mask;
        /* 
         *  We need the mask to align with the position in the word, which
         *  is (lsb) bits over to the left
         */
        clearMask = leftShift(mask, lsb);
        

        /* 
         * Want to clear out the 1s, so we can use the compliment 
         * to make all 1s except where we are putting it in
         */
        clearMask = complement(clearMask);
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
        mask = leftShift(mask, lsb);

        /* 
         *  Then, since it is cleared out by the steps above, 
         *  we can use | to get the values in, as it will be 
         *  1s if mask is 1, or the value if the mask is 0 
         *  which is 0 in the area we want to put the word in
         */
        word = word | mask;

        return word;
}


/*
 *  Name      : Bitpack_news
 *  Purpose   : Pack (value) using (width) bits at (lsb) into (word) for 
 *              signed values
 *  Parameters: (uint64_t) word  = The word to pack into
 *              (unsigned) width = The number of bits that (value) will use
 *              (unsigned) lsb   = The position of the least significant bit 
 *                                 of (value) in (word)
 *              (int64_t)  value = The value to be packed into the word
 *  Output    : (uint64_t) The word after packing value in
 *  Notes     : Will CRE if given a width greater than 64
 *              Will CRE if lsb is greater than 64
 *              Will CRE if width + lsb is greater than 64
 *              Will fully replace the word at width equal to 64
 *              Will return the word for width equal to 0
 */
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
        if (width == 0) {
                return word;
        }
        /* 
         * If it takes up the whole word, then just return it
         * Note that we do it this way, as the value doesn't fit into
         * uint64_t otherwise
         */
        if (width == MAX_WIDTH) {
                return (uint64_t)value;
        }

        /* Get rid of the excess 1s from 2's compliment */
        uint64_t unsignedValue = (uint64_t)value;
        unsignedValue = leftShift(unsignedValue, MAX_WIDTH - width);
        unsignedValue = rightShift(unsignedValue, MAX_WIDTH - width);

        return Bitpack_newu(word, width, lsb, unsignedValue);
}


/*
 *  Name      : leftShift
 *  Purpose   : Shift (word) to the left by (count) bits
 *  Parameters: (uint64_t) word  = The value to shift
 *              (unsigned) count = The number of bits to shift by
 *  Output    : (uint64_t) The word after shifting
 *  Notes     : Will return the word itself (do nothing) with count equal to 0
 *              Will return 0 with counter greater or equal to 64
 */
static uint64_t leftShift(uint64_t word, unsigned count)
{
        if (count == 0) {
                return word;
        }
        if (count >= MAX_WIDTH) {
                return 0;
        }
        return word << count;
        
}
/*
 *  Name      : rightShift
 *  Purpose   : Shift (word) to the left by (count) bits
 *  Parameters: (uint64_t) word  = The value to shift
 *              (unsigned) count = The number of bits to shift by
 *  Output    : (uint64_t) The word after shifting
 *  Notes     : Will return the word itself (do nothing) with count equal to 0
 *              Will return 0 with counter greater or equal to 64
 */
static uint64_t rightShift(uint64_t word, unsigned count)
{
        if (count == 0) {
                return word;
        }
        if (count >= MAX_WIDTH) {
                return 0;
        }
        return word >> count;
}

/*
 *  Name      : complement
 *  Purpose   : Return the bitwise complement of (word)
 *  Parameters: (uint64_t) word  = The value to find the bitwise complement of
 *  Output    : (uint64_t) The complement
 *  Notes     : (None)
 */
static uint64_t complement(uint64_t word)
{
        return ~word;
}

#undef MAX_WIDTH