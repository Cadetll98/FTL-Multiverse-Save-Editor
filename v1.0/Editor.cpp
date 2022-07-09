#include "Editor.h"
#include <iostream>
using namespace std;


bool readSavefile(FILE* in, Save& sav) {
    int fSize, crewSize;
    int counter = 0;
    int itr_nameAry = 0;
    unsigned char* itr_buf;
    unsigned char* buffer_byte;
    string strShipID, strTmp;

    // Go to EOF to get size of file in bytes
    fseek(in, 0, SEEK_END);
    fSize = ftell(in);
    rewind(in);

    // allocate buffer
    buffer_byte = (unsigned char*)malloc(fSize * sizeof(unsigned char));
    if (buffer_byte == NULL) {
        std::cout << "Unable to allocate memory!" << std::endl;
        return 1;
    }
    sav.savBuffer = buffer_byte;    // save buffer to class
    sav.bufSize = fSize;
    // read whole file to buffer
    fread(buffer_byte, fSize, 1, in);

    // Starts Data Extraction
    itr_buf = buffer_byte;  // buffer iterator

    // Read Header
    for (; itr_nameAry < 6; itr_nameAry++) {
        sav.mData.emplace(sav.itemName[itr_nameAry],to_string(readSize(&itr_buf)));
    }
    itr_buf += 8;   // skip 2 unknown value

    // Read Ship data
    for (; itr_nameAry < 8; itr_nameAry++) {
        readString(&itr_buf, strTmp);
        sav.mData.emplace(sav.itemName[itr_nameAry], strTmp);
    }
    sav.mData.emplace(sav.itemName[itr_nameAry], to_string(readSize(&itr_buf)));
    itr_nameAry++;
    itr_buf += 4;   // skip 1 unknown value

    // Read Score
    sav.mData.emplace(sav.itemName[itr_nameAry], to_string(readSize(&itr_buf)));
    itr_nameAry++;
    
    // Skip Score Detail and jump to Ship ID 2nd
    strTmp = "\0";
    strShipID = sav.mData.at(sav.itemName[7]);
    while (strTmp != strShipID && counter < MAXSCOREENTRY) {
        if (readString(&itr_buf, strTmp) == 1) {    // skip the unknown value 0xaa before Ship ID 
            continue;
        }
        readSize(&itr_buf);
        counter++;
    }
    if (strShipID != strTmp) {
        cout << "ShipID Not Detected!" << endl;
        return 1;
    }
    itr_buf -= 4;  // overread(ed) 4 byte from previous loop

    // skip Ship Name and Ship Graphical Name
    readString(&itr_buf, strTmp);
    readString(&itr_buf, strTmp);

    // skip Crew info
    crewSize = readSize(&itr_buf);
    for (int i = 0; i < crewSize; i++) {
        readString(&itr_buf, strTmp);
        readString(&itr_buf, strTmp);
    }

    // Read Resource Data
    itr_buf += 16;   // skip 4 * 4 bytes unknwon
    sav.hullPtr = itr_buf;  // save start of resource pointer to class
    for (; itr_nameAry < 15; itr_nameAry++) {
        sav.mData.emplace(sav.itemName[itr_nameAry], to_string(readSize(&itr_buf)));
    }

    return 0;
}

// update buffer in sav class and write buffer to file
bool writeSavefile(FILE* out, Save& sav) {
    int val;

    for (size_t j = 0; j < 5; j++, sav.hullPtr += 4) {
        val = stoi(sav.mData.at(sav.itemName[10 + j]));
        // update buffer with little endian
        for (size_t i = 0; i < 4; i++) {
            sav.hullPtr[i] = (val >> (8 * i)) & 0xFF;
        }
    }

    // write buffer to file
    val = fwrite(sav.savBuffer, sav.bufSize, 1, out);

    return val < 1 ? true : false;
}



// read next byte and convert to decimal as string size
int readSize(unsigned char** posPtr) {
    unsigned char* pos = *posPtr;
    int size = pos[3] * 16777216 + pos[2] * 65536 + pos[1] * 256 + pos[0];
    *posPtr += 4;
    return size;
}

// Read the next 4 bytes as # of bytes for string and read that string
// The read(ed) string is return using reference strRet, itrator is incremented as well.
bool readString(unsigned char** charBufPtr, string& strRet) {
    int sz = readSize(charBufPtr);
    if (sz < MAXSTRSIZE && sz > 0) {
        strRet.assign(reinterpret_cast<char*>(*charBufPtr), sz);
        *charBufPtr += sz;
        return 0;
    }
    return 1;
}

