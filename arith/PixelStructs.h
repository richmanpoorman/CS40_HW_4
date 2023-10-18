#include <stdint.h>

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
        uint64_t codeword;
} *Codeword;