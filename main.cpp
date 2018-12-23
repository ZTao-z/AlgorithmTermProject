#include "ReadFile.h"
#include "Structure.h"
#include "Algorithm_1.h"
#include "Algorithm_2.h"
#include <time.h> 

int main() {
    clock_t  clockBegin, clockEnd; 
    for(int i = 1; i <= 71; i++) {
        char file[20];
        cout << "p" << i << "|";
        sprintf(file, "./Instances/p%d", i);
        string a = file;
        Information info = ReadFile::readDataFile(a);
        clockBegin = clock();
        //int p = Greedy::run(info);
        int p = SA::run(info);
        clockEnd = clock();
        
        cout << p << "|" << double(clockEnd - clockBegin) / 1000.0f;
        cout << endl;
    }
    return 0;
}