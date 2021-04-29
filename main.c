#include "Processor.c"


int main(int argc, char** argv) {
    if (argc > 1) {
        Processor* proc = processor();

        if (loadFile(proc, argv[1])) {
            decode(proc);

            /* for (auto satellite : results) {
                cout<<"Satellite  "<<satellite.id<<" has sent bit "<<satellite.sentBit<<" (delta = "<<satellite.delta<<")"<<endl;
            } */
        }
    } else {
        printf("No file was parsed\n");
    }
    
    return 0;
}