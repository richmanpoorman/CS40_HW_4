
#define numBCDBits 4
#define numABits 9
#define maxFloat 0.3

#define maxBCDInt (1 << numBCDBits)
#define maxAInt (1 << numABits)
#define scaleFactor (maxBCDInt / maxFloat)

/*
 *  Name      : getMaxFloat
 *  Purpose   : Getter for the absolute value of the maximum float 
 *              representation for luma
 *  Parameters: None
 *  Output    : The largest float value that can be attributed to a luma
 */
float getMaxFloat() {
        return maxFloat;
}

/*
 *  Name      : getBCDLength
 *  Purpose   : Getter for the absolute value of the maximum int 
 *              representation for luma b, c, and d
 *  Parameters: None
 *  Output    : The largest int value that can be attributed to a b, c, 
 *              or d luma
 */
int getBCDLength() {
        return numBCDBits;
}

/*
 *  Name      : getALength
 *  Purpose   : Getter for the absolute value of the maximum int 
 *              representation for luma a
 *  Parameters: None
 *  Output    : The largest int value that can be attributed to luma a
 */
int getALength() {
        return numABits;
}

/*
 *  Name      : getScaleFactor
 *  Purpose   : Getter for the scale factor used to convert luma b, c, and d
 *              from floats to ints
 *  Parameters: None
 *  Output    : Int representing the scale factor used to convert luma
 *              b, c, and d from floats to ints */
int getScaleFactor() {
        return scaleFactor;
}

int getAMaxValue() {
        return maxAInt - 1;
}

int getBCDMaxValue() {
        return maxBCDInt - 1;
}

#undef numBCDBits 
#undef numABits 
#undef maxFloat

#undef maxInt 
#undef maxAInt 
#undef scaleFactor 