#pragma once

#include <iostream>

#include <stdio.h>
#include <stdlib.h>
using namespace std;
typedef int OffsetType;
#define OffsetByte 4096
class BufferManager_y
{
public:
    BufferManager_y();
    ~BufferManager_y();
    
    
    char* getBlockByOffset(string fileName, int offsetInFile);//yyr
    void updateBlock(string fileName, char *UpdateContent, int offsetInFile);
    void deleteBlock(string fileName, int offsetInFile);
    int createNewBlock(string fileName);
    void loadToArray(string filename);
    void writeBackDisk(string fileName);
    static char result[50000000];
    static int used_byte;
private:
    
    
};
extern BufferManager_y bm_y;






