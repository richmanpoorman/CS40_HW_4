/* 
 *   Name       : CompressionStep.h
 *   Assignment : CS40 Homework 4 (arith)
 *   Purpose    : Struct defining a generic step of the compression algorithm
 *                as composed to two opponent processes (compression vs.
 *                decompression)
 *   Editors    : Matthew Wong (mwong14), Ivi Fung (sfung02)
 */

#include <pnm.h>

#ifndef COMPRESSION_STEP_40_
#define COMPRESSION_STEP_40_

typedef struct CompressionStep {
        void (*compress)(Pnm_ppm image);
        void (*decompress)(Pnm_ppm image);
} *CompressionStep;

#endif