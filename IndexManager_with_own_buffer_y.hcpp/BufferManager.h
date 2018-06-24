/*
***************************************************************************************************************************
主要的外部接口：
1.insertPos getInsertPosition(Table& tabl);//返回插入数据的可行位置，传入的这个参有待商榷，因为不知道有没有table类
2.void setDirty(int bufferNum);//每次改动buffer后调用，标记被改动的block
3.void BufferManager::accessBuffer(int bufferNum);//每次读buffer时调用，用来标记使用时间
4.buffer类里的两个getcontent函数用来读取buffer里的内容
5.删除一个块后清理其缓存的方式是懒惰删除，即将符号位置为empty

注:每个buffer中存有多条元组，每个元组有（最大长度+1）位，多出来的1位即第0位是符号位，为empty(#define empty *)表示该行无内容或内容被删除

***************************************************************************************************************************
*/



#ifndef _BUFFERMANAGER_H
#define _BUFFERMANAGER_H

#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#define blockSize 4096
#define maxBufferNum 300

//为了区分，以下代码中内存缓冲区都用buffer表示，其对应的磁盘文件中的块用block表示

//一条记录插入时在内存中的位置
class Buffer{
public:
	bool isDirty; //脏为1
	bool isValid; //有用为1，无用为0
	string fileName;
	int offsetInFile; //这部分缓存对应的块在文件中的偏移量
	int time; //最近一次被访问的时间
	char content[blockSize];//buffer里的内容
	
	Buffer();
	void initialize();
	string getContent(int headPos,int tailPos);
	char getContent(int pos);
};
 
class BufferManager{
public:
	BufferManager();
	~BufferManager();
	void writtenToDisk(int bufferNum);//将内存中的buffer写入文件并初始化buffer
	int getBuffer(string fileName,int offsetInFile);//传入文件名和这个块在文件中的offset。返回其在内存中的编号
	void readToBuffer(string fileName,int offsetInFile,int bufferNum);//将文件中的块读取到内存指定的位置
	void setDirty(int bufferNum);//标记被改动的block
	void accessBuffer(int bufferNum);//每次访问buffer时调用，标记使用时间
	int getEmptyBuffer();//寻找内存中的空buffer
	int getEmptyBufferExcept(string fileName);//寻找内存中的空buffer，且不能替换掉给定的文件
	int countBlock(string fileName);
	int addBuffer(string fileName,int blockNum);//record插入新的block，返回新的block在内存中对应的buffer的编号
	int getInsertPosition(string fileName,int recordSize);//返回插入数据的可行位置
	void setInvalid(string filename);//删除表时用来将缓存懒惰删除
	void setInvalid(string filename,int offsetInFile);
	char* getBlockByOffset(string fileName,int offsetInFile);//yyr
	void updateBlock(string fileName, char *UpdateContent, int offsetInFile);
	void deleteBlock(string fileName, int offsetInFile);
	int createNewBlock(string fileName);
	friend class RecordManager;
	friend class indexManager;
//private:
	Buffer bufferPool[maxBufferNum];
};

extern BufferManager bm;
#endif
