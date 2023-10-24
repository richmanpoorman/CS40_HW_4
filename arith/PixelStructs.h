#include <stdint.h>

#ifndef PIXEL_STRUCTS_H_
#define PIXEL_STRUCTS_H_

/*
 *  Name   : Rgb_float
 *  Purpose: Represents a pixel in the RGB format as floats
 *  Fields : (float) red   = The red color as range [0, 1]
 *           (float) green = The green color as range [0, 1]
 *           (float) blue  = The blue color as range [0, 1]
 */
typedef struct Rgb_float {
        float red;
        float green;
        float blue;
} *Rgb_float;

/*
 *  Name   : Cie_float
 *  Purpose: Represents a pixel in the CIE format as floats
 *  Fields : (float) y  = The luma as range [0, 1]
 *           (float) pb = The blue difference as range [-0.5, 0.5]
 *           (float) pr = The red difference as range [-0.5, 0.5]
 */
typedef struct Cie_float {
        float y;
        float pb;
        float pr;
} *Cie_float;

/*
 *  Name   : Dct_float
 *  Purpose: Represents a 2x2 block of pixels after DCT as floats
 *  Fields : (float) a  = The a value as range [0, 1]
 *           (float) b  = The b value as range [-0.5, 0.5]
 *           (float) c  = The c value as range [-0.5, 0.5]
 *           (float) d  = The d value as range [-0.5, 0.5]
 *           (float) pb = The blue difference as range [-0.5, 0.5]
 *           (float) pr = The red difference as range [-0.5, 0.5]
 */
typedef struct Dct_float {
        float a;
        float b;
        float c;
        float d;
        float pb;
        float pr;
} *Dct_float;

/*
 *  Name   : Dct_int
 *  Purpose: Represents a 2x2 block of pixels after DCT as packable ints
 *  Fields : (int) a  = The a value packed into an int
 *           (int) b  = The b value packed into an int
 *           (int) c  = The c value packed into an int
 *           (int) d  = The d value packed into an int
 *           (int) pb = The blue difference packed into an int
 *           (int) pr = The red difference packed into an int
 *  Notes  : Determined by the codeword
 */
typedef struct Dct_int {
        int a;
        int b;
        int c;
        int d;
        int pb;
        int pr;
} *Dct_int;

/*
 *  Name   : Codeword
 *  Purpose: Represents a 2x2 block of pixels packed
 *  Fields : (uint32_t) codeword = The DCT data packed into 32 bits
 *  Notes  : Determined by the codeword
 */
typedef struct Codeword {
        uint32_t codeword;
} *Codeword;

float clampToRange(float input, float lower, float upper);
int   floatToInt(float input, int denominator, int lower, int upper);
float intToFloat(int input, int denominator, float lower, float upper);

#endif