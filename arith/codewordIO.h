#include <stdio.h>
#include <pnm.h>

#ifndef _CODEWORD_IO_H_
#define _CODEWORD_IO_H_

Pnm_ppm readCodewordFile(FILE *input);

void writeCodewordFile(Pnm_ppm codewordImage, FILE *output);

#endif