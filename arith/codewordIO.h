/* 
 *   Name       : codewordIO.h
 *   Assignment : CS40 Homework 4 (arith)
 *   Purpose    : Module for reading and writing codewords
 *   Editors    : Matthew Wong (mwong14), Ivi Fung (sfung02)
 */

#include <stdio.h>
#include <pnm.h>

#ifndef _CODEWORD_IO_H_
#define _CODEWORD_IO_H_

Pnm_ppm readCodewordFile(FILE *input);

void writeCodewordFile(Pnm_ppm codewordImage, FILE *output);

#endif