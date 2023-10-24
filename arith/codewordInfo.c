#include <stdint.h>

/* The Codeword table in the form {a, b, c, d, pb, pr} for width and lsb */
#define width {9 , 5 , 5 , 5, 4, 4}
#define lsb   {23, 18, 13, 8, 4, 0}
 
#define numBytes 4 
#define MAX_SIZE 64
/*
 *  Name      : getALength
 *  Purpose   : Getter for the number of bits used to represent luma a 
 *  Parameters: None
 *  Output    : The number of bits used to represent luma a
 */
int getALength() {
        int codewordWidths [6] = width;
        return codewordWidths[0];
}

/*
 *  Name      : getBLength
 *  Purpose   : Getter for the number of bits used to represent luma b
 *  Parameters: None
 *  Output    : The number of bits used to represent luma b
 */
int getBLength() {
       int codewordWidths [6] = width;
        return codewordWidths[1];
}

/*
 *  Name      : getCLength
 *  Purpose   : Getter for the number of bits used to represent luma c 
 *  Parameters: None
 *  Output    : The number of bits used to represent luma c
 */
int getCLength() {
        int codewordWidths [6] = width;
        return codewordWidths[2];
}

/*
 *  Name      : getDLength
 *  Purpose   : Getter for the number of bits used to represent luma d 
 *  Parameters: None
 *  Output    : The number of bits used to represent luma d
 */
int getDLength() {
        int codewordWidths [6] = width;
        return codewordWidths[3];
}

/*
 *  Name      : getPbLength
 *  Purpose   : Getter for the number of bits used to represent pb
 *  Parameters: None
 *  Output    : The number of bits used to represent pb
 */
int getPbLength() {
        int codewordWidths [6] = width;
        return codewordWidths[4];
}

/*
 *  Name      : getPrLength
 *  Purpose   : Getter for the number of bits used to represent pr
 *  Parameters: None
 *  Output    : The number of bits used to represent pr
 */
int getPrLength() {
        int codewordWidths [6] = width;
        return codewordWidths[5];
}

/*
 *  Name      : getALsb
 *  Purpose   : Getter for the least significant bit of a in the codeword
 *  Parameters: None
 *  Output    : The bit place of the lsb of a
 */
int getALsb() {
        int codewordLSBs [6] = lsb;
        return codewordLSBs[0];
}

/*
 *  Name      : getBLsb
 *  Purpose   : Getter for the least significant bit of b in the codeword
 *  Parameters: None
 *  Output    : The bit place of the lsb of b
 */
int getBLsb() {
        int codewordLSBs [6] = lsb;
        return codewordLSBs[1];
}

/*
 *  Name      : getCLsb
 *  Purpose   : Getter for the least significant bit of c in the codeword
 *  Parameters: None
 *  Output    : The bit place of the lsb of c
 */
int getCLsb() {
        int codewordLSBs [6] = lsb;
        return codewordLSBs[2];
}

/*
 *  Name      : getDLsb
 *  Purpose   : Getter for the least significant bit of d in the codeword
 *  Parameters: None
 *  Output    : The bit place of the lsb of d
 */
int getDLsb() {
        int codewordLSBs [6] = lsb;
        return codewordLSBs[3];
}

/*
 *  Name      : getPbLsb
 *  Purpose   : Getter for the least significant bit of pb in the codeword
 *  Parameters: None
 *  Output    : The bit place of the lsb of pb
 */
int getPbLsb() {
        int codewordLSBs [6] = lsb;
        return codewordLSBs[4];
}

int getPrLsb() {
        int codewordLSBs [6] = lsb;
        return codewordLSBs[5];
}

/*
 *  Name      : getAMaxValue
 *  Purpose   : Getter for the largest int that can be used to represent luma a
 *  Parameters: None
 *  Output    : The largest possible int used to represent luma a
 */
uint64_t getAMaxValue() {
        int aLength = getALength();

        if (aLength == MAX_SIZE) {
                uint64_t allOnes = 0;
                return ~allOnes;
        }

        return (1 << aLength) - 1;
}

/*
 *  Name      : getBMaxValue
 *  Purpose   : Getter for the largest int that can be used to represent luma b
 *  Parameters: None
 *  Output    : The largest possible int used to represent luma b
 */
uint64_t getBMaxValue() {

        int bLength = getBLength();

        if (bLength == MAX_SIZE) {
                uint64_t allOnes = 0;
                return ~allOnes;
        }
        
        return (1 << bLength) - 1;
}

/*
 *  Name      : getCMaxValue
 *  Purpose   : Getter for the largest int that can be used to represent luma c
 *  Parameters: None
 *  Output    : The largest possible int used to represent luma c
 */
uint64_t getCMaxValue() {
        int cLength = getCLength();

        if (cLength == MAX_SIZE) {
                uint64_t allOnes = 0;
                return ~allOnes;
        }

        return (1 << cLength) - 1;
}

/*
 *  Name      : getDMaxValue
 *  Purpose   : Getter for the largest int that can be used to represent luma d
 *  Parameters: None
 *  Output    : The largest possible int used to represent luma d
 */
uint64_t getDMaxValue() {
        int dLength = getDLength();

        if (dLength == MAX_SIZE) {
                uint64_t allOnes = 0;
                return ~allOnes;
        }

        return (1 << dLength) - 1;
}

/*
 *  Name      : getNumBytes
 *  Purpose   : Getter for the total byte size of the word
 *  Parameters: None
 *  Output    : The largest possible int used to represent luma d
 */
int getNumBytes()
{
        return numBytes;
}

#undef width
#undef lsb
#undef numBytes
#undef MAX_SIZE