#include <math.h>

float clampToRange(float input, float lower, float upper);
int   floatToInt(float input, int denominator, int lower, int upper);
float intToFloat(int input, int denominator, float lower, float upper);

float clampToRange(float input, float lower, float upper)
{
        if (input < lower) {
                input = lower;
        }
        if (input > upper) {
                input = upper;
        }
        return input;
}

int   floatToInt(float input, int denominator, int lower, int upper)
{
        
        double scaled = round(input * denominator);

        if (scaled > upper) {
                return upper;
        }
        if (scaled < lower) {
                return lower;
        }
        return (int)scaled;
}

float intToFloat(int input, int denominator, float lower, float upper)
{
        float value = 1.0 * input / denominator;
        value = clampToRange(value, lower, upper);
        return value;
}