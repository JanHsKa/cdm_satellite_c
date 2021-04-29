#include "Processor.h"
#include <stdio.h>


int main(int argc, char** argv) {
    if (argc > 1) {
        Processor* proc = processor();

        if (loadFile(proc, argv[1])) {
            decode(proc);
        }
    } else {
        printf("No file was parsed\n");
    }
    
    return 0;
}