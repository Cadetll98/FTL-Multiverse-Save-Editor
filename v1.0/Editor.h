#pragma once 
#include <string>
#include <map>
using namespace std;

#define MAXSCOREENTRY 17
#define MAXSTRSIZE 30

class Save {
public:
    string itemName[15] = { "Version", "Difficulty",  "Ship Defeated", "Jumps in Sector",
                            "Scrap Collected", "Crew Recruited", "Ship Name", "Ship ID",
                            "Current Sector", "Score", "Hull", "Fuel", "Missle", "Drone",
                            "Scrap"};
    map<string, string> mData;
    unsigned char* savBuffer = NULL;
    int bufSize = 0;
    unsigned char* hullPtr = NULL;

    ~Save() {
        if (savBuffer != NULL) {
            free(savBuffer);
        }
    }
    //destructor for buffer!
};

bool readSavefile(FILE* in, Save& sav);
bool writeSavefile(FILE* out, Save& sav);
int readSize(unsigned char** posPtr);
bool readString(unsigned char** charBufPtr, string& strRet);
//bool writeFile(FILE* out);

