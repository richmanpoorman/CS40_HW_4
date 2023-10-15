#include <pnm.h>

#ifndef COMPRESSION_STEP_40_
#define COMPRESSION_STEP_40_

typedef struct CompressionStep {
        void (*compress)(Pnm_ppm image);
        void (*decompress)(Pnm_ppm image);
} *CompressionStep;

#endif