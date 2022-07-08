#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define UNMATCH_WORD -99
#define MAX_SYS_WSIZE 15
#define NOMAX 50
#define SCORE_TYPES_COUNT 17
#define CREW_ATTRI 44

int readSize(unsigned char** posPtr);
int readString(unsigned char** charBufPtr, unsigned char** str2, int maxSize);

int main() {
    int strlen = 0;
    int fSize;
    int ch_eq = 99;
    int crewSize, crewSize_detail;
    unsigned char* charBuf;
    unsigned char* buffer_byte;
    unsigned char* shipID = NULL;

    FILE* in = fopen("hs_mv_continue.sav", "rb"); //"testinput.txt" "hs_mv_continue.sav"
    if (in == NULL) {
        printf("unable to open infile!");
        return 1;
    }
    FILE* out = fopen("hs_mv_continue_out.sav", "wb+");
    if (out == NULL) {
        printf("unable to open outfile!");
        return 1;
    }

    // Go to EOF to get size of file in bytes
    fseek(in, 0, SEEK_END);
    fSize = ftell(in);
    rewind(in);

    // allocate buffer
    buffer_byte = (unsigned char*)malloc(fSize * sizeof(unsigned char));
    if (buffer_byte == NULL) {
        printf("unable to allocated memory!");
        return 1;
    }
    // read whole file to buffer
    fread(buffer_byte, fSize, 1, in);


    // print Header 24 bytes
    charBuf = buffer_byte;
    printf("Version: 0x%x 0x%x 0x%x 0x%x\n", charBuf[0], charBuf[1], charBuf[2], charBuf[3]);
    charBuf += 4;
    printf("Difficulty: 0x%x 0x%x 0x%x 0x%x\n", charBuf[0], charBuf[1], charBuf[2], charBuf[3]);
    charBuf += 4;
    printf("Ship Defeated: 0x%x 0x%x 0x%x 0x%x\n", charBuf[0], charBuf[1], charBuf[2], charBuf[3]);
    charBuf += 4;
    printf("Jumps in Sector: 0x%x 0x%x 0x%x 0x%x\n", charBuf[0], charBuf[1], charBuf[2], charBuf[3]);
    charBuf += 4;
    printf("Scrap Collected: 0x%x 0x%x 0x%x 0x%x\n", charBuf[0], charBuf[1], charBuf[2], charBuf[3]);
    charBuf += 4;
    printf("Crew Recruited: 0x%x 0x%x 0x%x 0x%x\n", charBuf[0], charBuf[1], charBuf[2], charBuf[3]);
    charBuf += 4;
    printf("Unknown 1: 0x%x 0x%x 0x%x 0x%x\n", charBuf[0], charBuf[1], charBuf[2], charBuf[3]);
    charBuf += 4;
    printf("Unknown 2: 0x%x 0x%x 0x%x 0x%x\n", charBuf[0], charBuf[1], charBuf[2], charBuf[3]);
    charBuf += 4;
    printf("\n");

    // Ship Data
    printf("\nShip Name: ");
    readString(&charBuf, NULL, NOMAX);

    printf("\nShip ID: ");
    readString(&charBuf, &shipID, NOMAX);

    printf("\nCurrent Sector: 0x%x 0x%x 0x%x 0x%x", charBuf[0], charBuf[1], charBuf[2], charBuf[3]);
    charBuf += 4;
    printf("\nUnknown: 0x%x 0x%x 0x%x 0x%x", charBuf[0], charBuf[1], charBuf[2], charBuf[3]);
    charBuf += 4;

    // Score Total
    printf("\nScore: 0x%x 0x%x 0x%x 0x%x", charBuf[0], charBuf[1], charBuf[2], charBuf[3]);
    charBuf += 4;
    
    // Score Detail
    for (size_t i = 0; i < SCORE_TYPES_COUNT && ch_eq != 0 && ch_eq != UNMATCH_WORD; i++) {
        printf("\n");
        ch_eq = readString(&charBuf, &shipID, MAX_SYS_WSIZE);
        if (ch_eq != UNMATCH_WORD) {
            printf(": 0x%x 0x%x 0x%x 0x%x", charBuf[0], charBuf[1], charBuf[2], charBuf[3]);
            charBuf += 4;
        }
    }

    // Ship ID 2nd
    printf("\n");
    readString(&charBuf, NULL, NOMAX);
    // Ship Name 2nd
    printf("  ");
    readString(&charBuf, NULL, NOMAX);
    // Ship graphic Name
    printf("  ");
    readString(&charBuf, NULL, NOMAX);

    // Crew Overview
    crewSize = readSize(&charBuf);
    printf("\nNumber of Crew: %d", crewSize);
    for (int i = 0; i < crewSize; i++) {
        printf("\n");
        readString(&charBuf, NULL, NOMAX);
        printf(": ");
        readString(&charBuf, NULL, NOMAX);
    }

    // Resource
    printf("\nUnknown: 0x%x 0x%x 0x%x 0x%x", charBuf[0], charBuf[1], charBuf[2], charBuf[3]);
    charBuf += 4;
    printf("\nUnknown: 0x%x 0x%x 0x%x 0x%x", charBuf[0], charBuf[1], charBuf[2], charBuf[3]);
    charBuf += 4;
    printf("\nUnknown: 0x%x 0x%x 0x%x 0x%x", charBuf[0], charBuf[1], charBuf[2], charBuf[3]);
    charBuf += 4;
    printf("\nUnknown: 0x%x 0x%x 0x%x 0x%x", charBuf[0], charBuf[1], charBuf[2], charBuf[3]);
    charBuf += 4;
    printf("\nHull: %d", readSize(&charBuf));
    printf("\nFuel: %d", readSize(&charBuf));
    printf("\nDrone: %d", readSize(&charBuf));
    printf("\nMissle: %d", readSize(&charBuf));
    int scrap = readSize(&charBuf);
    printf("\nScraps: %d", scrap);
    
    // Testing for write back
    unsigned char* scrapPos = charBuf - 4;
    int cash = 300;
    int newAmt = scrap + cash;
    scrapPos[0] += newAmt; 
    if (newAmt > 256) {
        scrapPos[1] = newAmt / 256;
    }

    fwrite(buffer_byte, fSize, 1, out);




    // Crew Detail
    /*
    crewSize_detail = readSize(&charBuf);
    printf("\nDetailed Crew: %d", crewSize_detail);
    for (int i = 0; i < crewSize_detail; i++) {
        printf("\n");
        readString(&charBuf, NULL, NOMAX);
        printf(": ");
        readString(&charBuf, NULL, NOMAX);
        for (size_t i = 0; i < CREW_ATTRI; i++) {
            printf("\n   %d: 0x%x 0x%x 0x%x 0x%x", i, charBuf[0], charBuf[1], charBuf[2], charBuf[3]);
            charBuf += 4;
        }
        printf("\n");
        readString(&charBuf, NULL, NOMAX);
        printf(": ");
        readString(&charBuf, NULL, NOMAX);
        for (size_t i = 0; i < 28; i++) {
            printf("\n   %d: 0x%x 0x%x 0x%x 0x%x", i, charBuf[0], charBuf[1], charBuf[2], charBuf[3]);
            charBuf += 4;
        }
    }
    */

    fclose(in);
    fclose(out);

    return 0;
}

// read next byte and convert to decimal as string size
int readSize(unsigned char** posPtr) {
    unsigned char* pos = *posPtr;
    int size = pos[3] * 16777216 + pos[2] * 65536 + pos[1] * 256 + pos[0];
    *posPtr += 4;
    return size;
}

// read the next string using its preceding size bytes
// if str2 is valid, allocate and store the read string to str2 for writeback
int readString(unsigned char** charBufPtr, unsigned char** str2, int maxSize) {
    unsigned char str[100] = {'\0'};
    int sz = readSize(charBufPtr);
    // check for unrecognized size WORD, skip it if found
    if (sz > maxSize) {
        return UNMATCH_WORD;
    }
    // fetch text string and print
    memcpy(str, *charBufPtr, sz);
    printf("%s", str);
    *charBufPtr += sz;

    if (str2 != NULL && *str2 != NULL) {
        return strcmp(*str2, str);
    } 
    if (str2 != NULL && *str2 == NULL) {
        *str2 = malloc((strlen(str) + 1) * sizeof(unsigned char));
        strcpy(*str2, str);
    }
    return 0;
}