#include "Processor.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

Processor* processor() {
    Processor* processor = malloc(sizeof(Processor));
    processor->generator = goldCodeGenerator();
    createSatellites(processor);

    return processor;
}

void createSatellites(Processor* processor) {
    for (int i = 0; i < SATELLITE_COUNT; i++) {
        Satellite* satellite = malloc(sizeof(Satellite));
        satellite->id = i + 1;
        satellite->t = DURATIONS[i];
        generate(processor->generator, satellite->chipSequence, REGISTER_PAIRS[i][0], REGISTER_PAIRS[i][1]);
        createSatelliteSignal(satellite);
        processor->satellites[i] = satellite;
    }
}

void createSatelliteSignal(Satellite* satellite) {
    for (int i = 0; i < SIGNALSIZE; i++) {
        if (satellite->chipSequence[i] == 1) {
            satellite->signal[i] = 1;
        } else {
            satellite->signal[i] = -1;
        }
    }
}

void decode(Processor* processor) {
    for (int i = 0; i < SATELLITE_COUNT; i++) {
        if (checkSatelliteSignal(processor, processor->satellites[i]->id)) {
            printf("Satellite %*d has sent bit %d (delta = %d)\n", 2, processor->satellites[i]->id, 
            processor->satellites[i]->sentBit, processor->satellites[i]->delta);
        }
    }
}

bool checkSatelliteSignal(Processor* processor, unsigned char satelliteId) {
    for (int i = 0; i < SIGNALSIZE; i++) {
        if (checkSignal(processor, i, satelliteId)) {
            processor->satellites[satelliteId - 1]->delta = i;
            return true;
        }
    }

    return false;
}

bool checkSignal(Processor* processor, unsigned short start, unsigned char satelliteId) {
    int checkSum = 0;
    int index = start;

    for (int i = 0; i < SIGNALSIZE; i++) {
        checkSum += processor->signalData[index % SIGNALSIZE] * processor->satellites[satelliteId - 1]->signal[i];
        index++;
    } 

    checkSum /= SIGNALSIZE;

    switch (checkSum) {
        case 1:
            processor->satellites[satelliteId - 1]->sentBit = 1;
            return true;
            break;
        case -1:
            processor->satellites[satelliteId - 1]->sentBit = 0;
            return true;
        default:
            return false;
    }

    return false;
}

bool loadFile(Processor* processor, const char* filePath) {
    FILE* file = fopen(filePath, "r");

    if(file == NULL) {
        printf("Failed to open file \n");
        return false;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    char* fileBuffer = (char*)malloc(sizeof(char) * fileSize);

    size_t readSize = fread(fileBuffer, sizeof(char), (size_t)fileSize, file);

    if (readSize != fileSize) {
        printf("Failed to read file \n");
        return false;
    }

    int index = 0;
    bool nextNegative = false;
    char entry = ' ';

    for (int i = 0; i < fileSize; i++) {
        entry = (char)fileBuffer[i];

        if (entry == '-') {
            nextNegative = true;
        } else if (entry == 32) {
            nextNegative = false;
        } else {
            if (index >= SIGNALSIZE) {
                printf("File is to large to read \n");
                return false;
            }
            processor->signalData[index] = entry - '0';

            if (nextNegative) {
                processor->signalData[index] *= -1;
            }

            nextNegative = false;
            index++;
        }
    }

    fclose(file);
    free(fileBuffer);

    return true;
}