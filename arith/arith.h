/* 
 *   Name       : arith.h
 *   Assignment : CS40 Homework 4 (arith)
 *   Purpose    : Coordinates the sequencing of each compression step
 *   Editors    : Matthew Wong (mwong14), Ivi Fung (sfung02)
 */
 
#include <stdio.h>
#include <pnm.h>

#ifndef ARITH_H_
#define ARITH_H_


/*
 *  Name      : compress40Output
 *  Purpose   : Take the image and print the compressed output to output file
 *  Parameters: (FILE *) input  = The input file to read the image from
 *              (FILE *) output = The output file to print the image to
 *  Output    : (None)
 *  Notes     : Assumes that the file is open
 */
void compress40Output(FILE *input, FILE *output);

/*
 *  Name      : decompress40Output
 *  Purpose   : Take the compressed image and print the 
 *              decompressed output to output file
 *  Parameters: (FILE *) input  = The input file to read the image from
 *              (FILE *) output = The output file to print the image to
 *  Output    : (None)
 *  Notes     : Assumes that the file is open
 */
void decompress40Output(FILE *input, FILE *output);

/*
 *  Name      : compress40Image
 *  Purpose   : Takes an image and compresses it in-place
 *  Parameters: (Pnm_ppm) image = The image to compress
 *  Output    : (None)
 *  Notes     : Assumes that the Pnm_ppm is valid
 */
void compress40Image(Pnm_ppm image);

/*
 *  Name      : decompress40Image
 *  Purpose   : Takes an image and decompresses it in-place
 *  Parameters: (Pnm_ppm) image = The image to decompress
 *  Output    : (None)
 *  Notes     : Assumes that the Pnm_ppm is valid
 */
void decompress40Image(Pnm_ppm image);

#endif