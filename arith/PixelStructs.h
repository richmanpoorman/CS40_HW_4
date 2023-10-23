#include <stdint.h>

#ifndef PIXEL_STRUCTS_H_
#define PIXEL_STRUCTS_H_

typedef struct Rgb_float {
        float red;
        float green;
        float blue;
} *Rgb_float;

typedef struct Cie_float {
        float y;
        float pb;
        float pr;
} *Cie_float;

typedef struct Dct_float {
        float a;
        float b;
        float c;
        float d;
        float pb;
        float pr;
} *Dct_float;

typedef struct Dct_int {
        int a;
        int b;
        int c;
        int d;
        int pb;
        int pr;
} *Dct_int;

typedef struct Codeword {
        uint32_t codeword;
} *Codeword;

float clampToRange(float input, float lower, float upper);
int   floatToInt(float input, int denominator, int lower, int upper);
float intToFloat(int input, int denominator, float lower, float upper);

#endif