#pragma once
#include "GoldCodeGenerator.h"
#include <stdbool.h>


struct satellite {
    unsigned char sentBit;
    int id;
    unsigned short t;
    unsigned short delta;
    unsigned char chipSequence[SIGNALSIZE];
    char signal[SIGNALSIZE];
};

typedef struct satellite Satellite;

struct processor {
    char signalData[SIGNALSIZE];
    Satellite* satellites[SATELLITE_COUNT];
    GoldCodeGenerator* generator;
};

typedef struct processor Processor;

void createSatellites(Processor* processor);
void createSatelliteSignal(Satellite* satellite);
void decode(Processor* processor);
bool checkSignal(Processor* processor, unsigned short start, unsigned char satelliteId);
bool checkSatelliteSignal(Processor* processor, unsigned char satelliteId);
bool loadFile(Processor* processor, const char* filePath);
Processor* processor();