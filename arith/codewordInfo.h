/* 
 *   Name       : codewordInfo.h
 *   Assignment : CS40 Homework 4 (arith)
 *   Purpose    : Getters for tje sizes needed for quantization 
 *                and codeword packing
 *   Editors    : Matthew Wong (mwong14), Ivi Fung (sfung02)
 */

#include <stdint.h>

float getMaxFloat();

int getALength();
int getBLength();
int getCLength();
int getDLength();
int getPbLength();
int getPrLength();

uint64_t getAMaxValue();
uint64_t getBMaxValue();
uint64_t getCMaxValue();
uint64_t getDMaxValue();

int getALsb();
int getBLsb();
int getCLsb();
int getDLsb();
int getPbLsb();
int getPrLsb();

int getNumBytes();