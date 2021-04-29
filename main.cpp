#include <iostream>
#include "Processor.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc > 1) {
        Processor processor = Processor();

        if (processor.loadFile(argv[1])) {
            vector<Satellite> results = processor.decode();

            for (auto satellite : results) {
                cout<<"Satellite  "<<satellite.id<<" has sent bit "<<satellite.sentBit<<" (delta = "<<satellite.delta<<")"<<endl;
            }
        }
    } else {
        cout<<"No file was parsed"<<endl;
    }
    
    return 0;
}