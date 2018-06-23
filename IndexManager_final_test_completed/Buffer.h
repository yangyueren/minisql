#pragma once

#include <iostream>

#include <stdio.h>
#include <stdlib.h>
using namespace std;
typedef int OffsetType;
#define OffsetByte 4096
class BufferManager
{
public:
	BufferManager();
	~BufferManager();
	char* Get_Node(OffsetType offset);
	void Update_Node(OffsetType offset, char * Content);
	OffsetType Create_Node();
	void Delete_Node(OffsetType offset);
	void UpdateIndex(char * Content);
	char* GetIndexHead();

	char* getBlockByOffset(string fileName, int offsetInFile);//yyr
	void updateBlock(string fileName, char *UpdateContent, int offsetInFile);
	void deleteBlock(string fileName, int offsetInFile);
	int createNewBlock(string fileName);

	static char result[50000000];
	static int used_byte;
private:


};






