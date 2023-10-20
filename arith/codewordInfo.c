#include "codewordInfo.h"

#define numBCDBits 4
#define numABits 9
#define maxFloat 0.3

#define maxInt (1 << numBCDBits)
#define maxAInt (1 << numABits)
#define scaleFactor (maxInt / maxFloat)

int getBCDLength() {
        return maxInt - 1;
}

int getALength() {
        return maxAInt - 1;
}

int getScaleFactor() {
        return scaleFactor;
}