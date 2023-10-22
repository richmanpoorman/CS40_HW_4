
#define widths {9, 4, 4, 4}
#define numABits 9
#define numBBits 4
#define numCBits 4
#define numDBits 4
#define maxFloat 0.3

#define maxAInt (1 << numABits)
#define maxBInt (1 << numBBits)
#define maxCInt (1 << numCBits)
#define maxDInt (1 << numDBits)

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
 *  Name      : getALength
 *  Purpose   : Getter for the number of bits used to represent luma a 
 *  Parameters: None
 *  Output    : The number of bits used to represent luma a
 */
int getALength() {
        return numABits;
}

/*
 *  Name      : getBLength
 *  Purpose   : Getter for the number of bits used to represent luma b
 *  Parameters: None
 *  Output    : The number of bits used to represent luma b
 */
int getBLength() {
        return numBBits;
}

/*
 *  Name      : getCLength
 *  Purpose   : Getter for the number of bits used to represent luma c 
 *  Parameters: None
 *  Output    : The number of bits used to represent luma c
 */
int getCLength() {
        return numCBits;
}

/*
 *  Name      : getDLength
 *  Purpose   : Getter for the number of bits used to represent luma d 
 *  Parameters: None
 *  Output    : The number of bits used to represent luma d
 */
int getDLength() {
        return numCBits;
}

/*
 *  Name      : getAMaxValue
 *  Purpose   : Getter for the largest int that can be used to represent luma a
 *  Parameters: None
 *  Output    : The largest possible int used to represent luma a
 */
int getAMaxValue() {
        return maxAInt - 1;
}

/*
 *  Name      : getBMaxValue
 *  Purpose   : Getter for the largest int that can be used to represent luma b
 *  Parameters: None
 *  Output    : The largest possible int used to represent luma b
 */
int getBMaxValue() {
        return maxBInt - 1;
}

/*
 *  Name      : getCMaxValue
 *  Purpose   : Getter for the largest int that can be used to represent luma c
 *  Parameters: None
 *  Output    : The largest possible int used to represent luma c
 */
int getCMaxValue() {
        return maxCInt - 1;
}

/*
 *  Name      : getDMaxValue
 *  Purpose   : Getter for the largest int that can be used to represent luma d
 *  Parameters: None
 *  Output    : The largest possible int used to represent luma d
 */
int getDMaxValue() {
        return maxDInt - 1;
}


#undef numABits 
#undef numBBits 
#undef numCBits 
#undef numDBits 
#undef maxFloat 

#undef maxAInt 
#undef maxBInt
#undef maxCInt 
#undef maxDInt