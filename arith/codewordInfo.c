

/* The Codeword table in the form {a, b, c, d, pb, pr} for width and lsb */
#define width {9 , 5 , 5 , 5, 4, 4}
#define lsb   {23, 18, 13, 8, 4, 0}



/*
 *  Name      : getALength
 *  Purpose   : Getter for the number of bits used to represent luma a 
 *  Parameters: None
 *  Output    : The number of bits used to represent luma a
 */
int getALength() {
        int codewordWidths [6] = width;
        return codewordWidths[0];
}

/*
 *  Name      : getBLength
 *  Purpose   : Getter for the number of bits used to represent luma b
 *  Parameters: None
 *  Output    : The number of bits used to represent luma b
 */
int getBLength() {
       int codewordWidths [6] = width;
        return codewordWidths[1];
}

/*
 *  Name      : getCLength
 *  Purpose   : Getter for the number of bits used to represent luma c 
 *  Parameters: None
 *  Output    : The number of bits used to represent luma c
 */
int getCLength() {
        int codewordWidths [6] = width;
        return codewordWidths[2];
}

/*
 *  Name      : getDLength
 *  Purpose   : Getter for the number of bits used to represent luma d 
 *  Parameters: None
 *  Output    : The number of bits used to represent luma d
 */
int getDLength() {
        int codewordWidths [6] = width;
        return codewordWidths[3];
}

/*
 *  Name      : getPbLength
 *  Purpose   : Getter for the number of bits used to represent pb
 *  Parameters: None
 *  Output    : The number of bits used to represent pb
 */
int getPbLength() {
        int codewordWidths [6] = width;
        return codewordWidths[4];
}

/*
 *  Name      : getPrLength
 *  Purpose   : Getter for the number of bits used to represent pr
 *  Parameters: None
 *  Output    : The number of bits used to represent pr
 */
int getPrLength() {
        int codewordWidths [6] = width;
        return codewordWidths[5];
}

/*
 *  Name      : getALsb
 *  Purpose   : Getter for the least significant bit of a in the codeword
 *  Parameters: None
 *  Output    : The bit place of the lsb of a
 */
int getALsb() {
        int codewordLSBs [6] = width;
        return codewordLSBs[0];
}

/*
 *  Name      : getBLsb
 *  Purpose   : Getter for the least significant bit of b in the codeword
 *  Parameters: None
 *  Output    : The bit place of the lsb of b
 */
int getBLsb() {
        int codewordLSBs [6] = width;
        return codewordLSBs[1];
}

/*
 *  Name      : getCLsb
 *  Purpose   : Getter for the least significant bit of c in the codeword
 *  Parameters: None
 *  Output    : The bit place of the lsb of c
 */
int getCLsb() {
        int codewordLSBs [6] = width;
        return codewordLSBs[2];
}

/*
 *  Name      : getDLsb
 *  Purpose   : Getter for the least significant bit of d in the codeword
 *  Parameters: None
 *  Output    : The bit place of the lsb of d
 */
int getDLsb() {
        int codewordLSBs [6] = width;
        return codewordLSBs[3];
}

/*
 *  Name      : getPbLsb
 *  Purpose   : Getter for the least significant bit of pb in the codeword
 *  Parameters: None
 *  Output    : The bit place of the lsb of pb
 */
int getPbLsb() {
        int codewordLSBs [6] = width;
        return codewordLSBs[4];
}

int getPrLsb() {
        int codewordLSBs [6] = width;
        return codewordLSBs[5];
}

/*
 *  Name      : getAMaxValue
 *  Purpose   : Getter for the largest int that can be used to represent luma a
 *  Parameters: None
 *  Output    : The largest possible int used to represent luma a
 */
int getAMaxValue() {
        unsigned aLength = getALength();
        return (1 << aLength) - 1;
}

/*
 *  Name      : getBMaxValue
 *  Purpose   : Getter for the largest int that can be used to represent luma b
 *  Parameters: None
 *  Output    : The largest possible int used to represent luma b
 */
int getBMaxValue() {
        unsigned bLength = getBLength();
        return (1 << bLength) - 1;
}

/*
 *  Name      : getCMaxValue
 *  Purpose   : Getter for the largest int that can be used to represent luma c
 *  Parameters: None
 *  Output    : The largest possible int used to represent luma c
 */
int getCMaxValue() {
        unsigned cLength = getCLength();
        return (1 << cLength) - 1;
}

/*
 *  Name      : getDMaxValue
 *  Purpose   : Getter for the largest int that can be used to represent luma d
 *  Parameters: None
 *  Output    : The largest possible int used to represent luma d
 */
int getDMaxValue() {
        unsigned dLength = getDLength();
        return (1 << dLength) - 1;
}


#undef width
#undef lsb