/* 
 *   Name       : PixelStructs.c
 *   Assignment : CS40 Homework 4 (arith)
 *   Purpose    : Defines the representations of values stored in each
 *                pixel of an image during each compression step, as well
 *                as helper functions to keep these values in range
 *   Editors    : Matthew Wong (mwong14), Ivi Fung (sfung02)
 */

#include <math.h>

float clampToRange(float input, float lower, float upper);
int   floatToInt(float input, int denominator, int lower, int upper);
float intToFloat(int input, int denominator, float lower, float upper);

/*
 *  Name      : clampToRange
 *  Purpose   : Takes a float, and forces it within the range
 *  Parameters: (float) input = The float to bound
 *              (float) lower = The lower bound of the range
 *              (float) upper = The upper bound of the range
 *  Output    : (float) The value of input if it is in the range, 
 *                      or the corresponding bound if it is out of range
 */
float clampToRange(float input, float lower, float upper)
{
        if (input < lower) {
                return lower;
        }
        if (input > upper) {
                return upper;
        }
        return input;
}

/*
 *  Name      : floatToInt
 *  Purpose   : Takes a float and turns it into an int, represented 
 *              approximately by input / denominator
 *  Parameters: (float) input       = The starting float to scale
 *              (int)   denominator = The scale factor
 *              (int)   lower       = The lower range the int can take
 *              (int)   upper       = The upper range the int can take
 *  Output    : (int) The value scaled by denominator, put into an 
 *                    int instead of a float
 */


int   floatToInt(float input, int denominator, int lower, int upper)
{
        
        int scaled = (int)round(input * denominator);

        if (scaled > upper) {
                return upper;
        }
        if (scaled < lower) {
                return lower;
        }
        return scaled;
}

/*
 *  Name      : intToFloat
 *  Purpose   : Takes a int and turns it into an float, represented 
 *              approximately by input / denominator
 *  Parameters: (int)   input       = The starting int to scale
 *              (int)   denominator = The scale factor to divide by
 *              (float) lower       = The lower range the float can take
 *              (float) upper       = The upper range the float can take
 *  Output    : (int) The value scaled by denominator, put into an 
 *                    float instead of (input / denominator)
 */
float intToFloat(int input, int denominator, float lower, float upper)
{
        float value = (1.0 * input) / denominator;
        value = clampToRange(value, lower, upper);
        return value;
}