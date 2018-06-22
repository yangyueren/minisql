#pragma once
#pragma once
# include <iostream>
# include <string>
# include <cstdio>
# include "BlockNode.h"
# define BLOCK_LEN			64	
# define MAX_BLOCK			400

using namespace std;

class BufferManager
{
private:
	fileNode * fileList;
	blockNode blockPool[MAX_BLOCK];
	int curBlockNum;
	int curFileNum;
	fileNode *getFile(string fileName);
	blockNode *getBlock(string fileName, int position);
	void writeToDisk(blockNode *block);
	void clearBlockNode(blockNode *block);
	void clearFileNode(fileNode *file);
	void writeAllToDisk();
public:
	BufferManager();
	//void deleteFileNode(string fileName);//delete file node when the file is deleted
	//blockNode *getHeadBlock(string fileName);//get head block of the file for record
	//blockNode *getNextBlock(string fileName, blockNode *block);//get the next of the block in the file
	//int getUsedSize(blockNode *block);//return block->usedSize
	//static void updateBlock(string fileName, char *newData, int offset, int addSize = BLOCK_LEN);//update
	//static void Delete_Block(string fileName, int offset);//delete the block for index
	//static int createNewBlock(string fileName);//create the block for index
	//static char *getContent(blockNode *block);//return block->content
	//static blockNode *getBlockByOffset(string fileName, int offset);
	void updateBlock(string file_name, char *UpdateContent, int offset);
	void deleteBlock(string file_name, int offset);
	char* getBlockByOffset(string file_name, int offset);
	int createNewBlock(string fileName);//create the block for index
	~BufferManager();
};


/*
void updateBlock(string file_name, char *UpdateContent, int offset);
void deleteBlock(sting file_name, int offset);
char* getBlockByOffset(string file_name, int offset);
int createNewBlock(string fileName);//create the block for index
*/