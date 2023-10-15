#include <stdbool.h>
#include <stdint.h>
#include "except.h"

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
        uint64_t maxValue = 1 << width;
        return n < maxValue;
}
bool Bitpack_fitss( int64_t n, unsigned width) 
{
        assert(width <= MAX_WIDTH);
        if (width == MAX_WIDTH) {
                return true;
        }
        if (width <= 1) {
                return false;
        }
        if (n < 0) {
                n = ~n;
        }
        uint64_t maxValue = 1 << (width - 1);
        return n < maxValue;
}
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= MAX_WIDTH);
        assert(lsb <= MAX_WIDTH);
        assert(width + lsb <= MAX_WIDTH);

        /* Get a line of all 1s */
        uint64_t mask = ~0;
        
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
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= MAX_WIDTH);
        assert(width >= 1);
        assert(lsb <= MAX_WIDTH);
        assert(width + lsb <= MAX_WIDTH);
        
        /* 
         *  The cast here stays within range, as since it is taking
         *  width - 1, it will fit into a int64_t
         */
        int64_t  value = Bitpack_getu(word, width - 1, lsb);
        uint64_t sign  = Bitpack_getu(word, 1        , lsb + width - 1);
        if (sign > 0) {
                /*
                 *  If it is in the form of 2's complement,
                 *  then instead we start with all 1's and 
                 *  remove the 1's where the value is 0 as bits
                 */
                value = ~0 & value;
        }
        return value;
}
// uint64_t Bitpack_newu(uint64_t word, unsigned width, 
//                       unsigned lsb, uint64_t value)
// {
// 
// }
// uint64_t Bitpack_news(uint64_t word, unsigned width, 
//                       unsigned lsb,  int64_t value);

#undef MAX_WIDTH