#pragma once
#include "SatelliteConfig.h"
#define SIGNALSIZE 1023


struct shiftRegister {
    unsigned short registerBits;
    unsigned short shiftBits[6];
    unsigned short resultBits[2];
    unsigned char shiftBitSize;
    unsigned char resultBitSize ;

};

typedef struct shiftRegister ShiftRegister;

struct goldCodeGenerator {
    ShiftRegister *g1;
    ShiftRegister *g2;
};

typedef struct goldCodeGenerator GoldCodeGenerator;


void generate(GoldCodeGenerator* codeGenerator, unsigned char* code, 
    unsigned short first, unsigned short second);
GoldCodeGenerator* goldCodeGenerator();
unsigned char get_next_bit(GoldCodeGenerator* generator);
unsigned char shift(ShiftRegister* shiftRegister);
unsigned short applyBitmasks(const unsigned short* bitmasks, unsigned char size, 
    unsigned short registerBits);
void reset(GoldCodeGenerator* generator);