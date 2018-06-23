#pragma once

#include <iostream>

#include <stdio.h>
#include <stdlib.h>
using namespace std;
typedef int OffsetType;
#define OffsetByte 4096
class Buffer
{
public:
	Buffer();
	~Buffer();
	char* Get_Node(OffsetType offset);
	void Update_Node(OffsetType offset, char * Content);
	OffsetType Create_Node();
	void Delete_Node(OffsetType offset);
	void UpdateIndex(char * Content);
	char* GetIndexHead();
	static char result[50000000];
	static int used_byte;
private:


};






