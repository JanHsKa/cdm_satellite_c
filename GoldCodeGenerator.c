#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

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


void generate(GoldCodeGenerator* codeGenerator, unsigned char* code, unsigned short first, unsigned short second);
GoldCodeGenerator* goldCodeGenerator();
unsigned char get_next_bit(GoldCodeGenerator* generator);
unsigned char shift(ShiftRegister* shiftRegister);
unsigned short applyBitmasks(const unsigned short* bitmasks, unsigned char size, unsigned short registerBits);
void reset(GoldCodeGenerator* generator);

GoldCodeGenerator* goldCodeGenerator() {
    ShiftRegister* g1 = malloc(sizeof(ShiftRegister));
    ShiftRegister* g2 = malloc(sizeof(ShiftRegister));

    GoldCodeGenerator* generator = malloc(sizeof(GoldCodeGenerator));
    g1->registerBits = 0b1111111111;
    g1->shiftBits[0] = 0b0010000000;
    g1->shiftBits[1] = 0b0000000001;
    g1->resultBits[0] = 0b00000000001;
    g1->shiftBitSize = 2;
    g1->resultBitSize = 1;


    unsigned short shiftBits[6] = {0b0100000000, 0b0010000000, 0b0000010000, 0b0000000100, 0b0000000010, 0b0000000001};
    
    for (int i = 0; i < 6; i++) {
        g2->shiftBits[i] = shiftBits[i];
        g2->shiftBitSize++;

    }

    g2->registerBits = 0b1111111111;
    g2->resultBitSize = 2;


    generator->g1 = g1;
    generator->g2 = g2;

    return generator;
}

void generate(GoldCodeGenerator* codeGenerator, unsigned char* code, unsigned short first, unsigned short second) {
    reset(codeGenerator);
    codeGenerator->g2->resultBits[0] = (1 << (10 - first));
    codeGenerator->g2->resultBits[1] = (1 << (10 - second));

    for (int i = 0; i <= SIGNALSIZE; i++) {
        code[i] = get_next_bit(codeGenerator);
    }
}

unsigned char get_next_bit(GoldCodeGenerator* generator) {
    unsigned char shift1 = shift(generator->g1);
    unsigned char shift2 = shift(generator->g2);

    return shift1 ^ shift2;
}

unsigned char shift(ShiftRegister* shiftRegister) {
    unsigned char resultBit = (unsigned char)applyBitmasks(shiftRegister->resultBits, shiftRegister->resultBitSize, shiftRegister->registerBits);
    unsigned short shiftBit = applyBitmasks(shiftRegister->resultBits, shiftRegister->shiftBitSize, shiftRegister->registerBits);


    shiftRegister->registerBits >>= 1;
    shiftRegister->registerBits |= shiftBit << 9;

    return resultBit;
}

unsigned short applyBitmasks(const unsigned short* bitmasks, unsigned char size, unsigned short registerBits) {
    unsigned short resultBit = 0;
    unsigned short helpBit = 0;

    for (int i = 0; i < size; i++) {
         helpBit = registerBits & bitmasks[i];
         while (helpBit >= 2) {
             helpBit >>= 1;
         }
         resultBit ^= helpBit;
    }

    return resultBit;
}

void reset(GoldCodeGenerator* generator) {
    generator->g1->registerBits = 0b1111111111;
    generator->g2->registerBits = 0b1111111111;

    for (int i = 0; i < generator->g2->resultBitSize; i++) {
        generator->g2->resultBits[i] = 0;
    }
}
