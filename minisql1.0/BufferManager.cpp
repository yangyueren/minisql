#include "BufferManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#define empty '*'
#define no_empty 1

Buffer::Buffer(){
	initialize();
}

void Buffer::initialize(){
	isDirty=0; //脏为1
	isValid=0; //有用为1，无用为0
	fileName="NULL";
	offsetInFile=0; //该块在文件中的偏移量
	time=0; //最近一次被访问的时间
	memset(content,empty,blockSize-1);
	content[blockSize]='\0';
}
string Buffer::getContent(int headPos,int tailPos){
	string tmp="";
	if(headPos>=0&&headPos<=tailPos&&tailPos<=blockSize-1)
		for(int i=headPos;i<tailPos;i++)
			tmp+=content[i];
	return tmp;
}
char Buffer::getContent(int pos){
	if(pos>=0&&pos<=blockSize-1)
		return content[pos];
	else return '\0';
}



BufferManager::BufferManager(){
	for(int i=0;i<maxBufferNum;i++)
		bufferPool[i].initialize();
}

BufferManager::~BufferManager(){
	for(int i=0;i<maxBufferNum;i++)
		writtenToDisk(i);
}

//将内存中的buffer写入文件并初始化buffer
void BufferManager::writtenToDisk(int bufferNum){
//不脏的话直接返回
	if(!bufferPool[bufferNum].isDirty) return;
//脏的情况
	string fileName=bufferPool[bufferNum].fileName;
	FILE *fp=NULL;
	if((fp=fopen((fileName).c_str(),"rb+"))==NULL){
		cout<<"Error:Opening file fails when written to disk!"<<endl;
		return;
	}
	else{
		if(fseek(fp,blockSize*bufferPool[bufferNum].offsetInFile,0)!=0){
			//文件头0(SEEK_SET)，当前位置1(SEEK_CUR)，文件尾2(SEEK_END)
			cout<<"Error:Seeking fails when written to disk!"<<endl;
			return;
		}
		else{
			if(fwrite(bufferPool[bufferNum].content,blockSize,1,fp)!=1){
				cout<<"Error:Writing fails when written to disk!"<<endl;
				return;
			}
		}
		fclose(fp);
		bufferPool[bufferNum].initialize();
	}
}

//传入文件名和这个块在文件中的offset。返回其在内存中的编号
int BufferManager::getBuffer(string fileName,int offsetInFile){
	int flag=0;//这个块不在内存中则为0,在则为1
	int bufferNum;
	for(bufferNum=0;bufferNum<maxBufferNum;bufferNum++)
		if(bufferPool[bufferNum].fileName==fileName&&bufferPool[bufferNum].offsetInFile==offsetInFile){
			flag=1;
			break;
		}
	if(0==flag){
		bufferNum=getEmptyBufferExcept(fileName);
		readToBuffer(fileName,offsetInFile,bufferNum);
	}
	accessBuffer(bufferNum);
	return bufferNum;
}

//将文件中的块读取到内存指定的位置
void BufferManager::readToBuffer(string fileName,int offsetInFile,int bufferNum){
	bufferPool[bufferNum].fileName=fileName;
	bufferPool[bufferNum].offsetInFile=offsetInFile;
	bufferPool[bufferNum].time=clock();

	FILE *fp=NULL;
	if((fp=fopen(fileName.c_str(),"rb"))==NULL){
		cout<<"Error:Opening file fails when read to buffer!"<<endl;
		return;
	}
	else{
		if(fseek(fp,blockSize*offsetInFile,0)!=0){
			//文件头0(SEEK_SET)，当前位置1(SEEK_CUR)，文件尾2(SEEK_END)
			cout<<"Error:Seeking fails when read to buffer!"<<endl;
			return;
		}
		else{
			fread(bufferPool[bufferNum].content,blockSize,1,fp);
		}
		fclose(fp);
	}
}

//标记被改动的block
void BufferManager::setDirty(int bufferNum){
	bufferPool[bufferNum].isDirty=1;
	accessBuffer(bufferNum);
}

//每次访问buffer时调用，标记使用时间
void BufferManager::accessBuffer(int bufferNum){
	bufferPool[bufferNum].time=clock();
}

//寻找内存中的空buffer
int BufferManager::getEmptyBuffer(){
	int replacedBufferNum=0;//被替换的块，所有块中最长时间未被访问
	int i;
	for(i=0;i<maxBufferNum;i++){
		if(!bufferPool[i].isValid){
			//找到无用的块
			bufferPool[i].initialize();
			bufferPool[i].isValid=1;
			return i;
		}
		if(bufferPool[i].time<bufferPool[replacedBufferNum].time)
			replacedBufferNum=i;
	}
	writtenToDisk(replacedBufferNum);//这个函数在写回磁盘后会将buffer初始化
	bufferPool[i].isValid=1;
	return replacedBufferNum;
}

//寻找内存中的空buffer，且不能替换掉给定的文件
int BufferManager::getEmptyBufferExcept(string fileName){
	int replacedBufferNum=0;//被替换的块，所有块中最长时间未被访问
	for(int i=0;i<maxBufferNum;i++){
		if(!bufferPool[i].isValid){
			//找到无用的块
			bufferPool[i].initialize();
			bufferPool[i].isValid=1;
			return i;
		}
		if(bufferPool[i].time<bufferPool[replacedBufferNum].time&&bufferPool[i].fileName!=fileName)
			replacedBufferNum=i;
	}
	writtenToDisk(replacedBufferNum);//这个函数在写回磁盘后会将buffer初始化
	bufferPool[replacedBufferNum].isValid=1;
	return replacedBufferNum;
}
//计算文件中块数
int BufferManager::countBlock(string fileName){
	FILE *fp=NULL;
	int size;
	if((fp=fopen(fileName.c_str(),"rb"))==NULL){
		cout<<"Error:Opening file fails when read to buffer!"<<endl;
		return -1;
	}
	else{
		if(fseek(fp,0L,SEEK_END)!=0){
			//文件头0(SEEK_SET)，当前位置1(SEEK_CUR)，文件尾2(SEEK_END)
			cout<<"Error:Seeking fails when read to buffer!"<<endl;
			return -1;
		}
		else{
			size=ftell(fp);//字节数？
		}
		fclose(fp);
	}
	int blockNum=size*8/blockSize+1;
	return blockNum;
}
//文件插入新的block，返回新的block在内存中对应的buffer的编号
int BufferManager::addBuffer(string fileName,int blockNum){
		int bufferNum=getEmptyBuffer();
		setDirty(bufferNum);
		bufferPool[bufferNum].fileName=fileName;
		bufferPool[bufferNum].offsetInFile=blockNum;
		return bufferNum;
}

//返回插入数据的可行buffer
int BufferManager::getInsertPosition(string fileName,int recordSize){
	int blockNum=countBlock(fileName);
	int bufferNum;
	if(blockNum==0){
		//新表没有块
		bufferNum=addBuffer(fileName,blockNum);
		return bufferNum;
	}
	else{
		int offsetInFile=blockNum-1;
		bufferNum=getBuffer(fileName,offsetInFile);
		int maxTupleNum=blockSize/recordSize;//每个block对应的buffer最大放下的元组数
		for(int i=0;i<maxTupleNum;i++){
			int position=i*recordSize;
			char isEmpty=bufferPool[bufferNum].content[position];
			if(isEmpty==empty){
				//找到一个可以插入的空行
				return bufferNum;
			}
		}
		//最后一个block对应的buffer是装满的，重新增加一个buffer
		bufferNum=addBuffer(fileName,blockNum);
		return bufferNum;
	}
}
//删除表时用来将缓存懒惰删除
void BufferManager::setInvalid(string filename){
	for(int i=0;i<maxBufferNum;i++){
		if(bufferPool[i].fileName==filename){
			bufferPool[i].isValid=0;
			bufferPool[i].isDirty=0;
		}
	}
}
void BufferManager::setInvalid(string filename,int offsetInFile){
	for(int i=0;i<maxBufferNum;i++){
		if(bufferPool[i].fileName==filename){
			bufferPool[i].isValid=0;
			bufferPool[i].isDirty=0;
		}
	}
}
/////yyr
char* BufferManager::getBlockByOffset(string fileName,int offsetInFile){
	int offsetInBuffer=getBuffer(fileName,offsetInFile);
	char* tmp=(char*)malloc(blockSize*sizeof(char));
	strcpy(tmp,bufferPool[offsetInBuffer].content);
	accessBuffer(offsetInBuffer);
	return tmp;	
}
void BufferManager::updateBlock(string fileName, char *UpdateContent, int offsetInFile){
	int offsetInBuffer=getBuffer(fileName,offsetInFile);
	strcpy(bufferPool[offsetInBuffer].content,UpdateContent);
	setDirty(offsetInBuffer);
}
void BufferManager::DeleteBlock(string fileName, int offsetInFile){//清除index缓存
	int flag=0;
	int bufferNum;
	for(bufferNum=0;bufferNum<maxBufferNum;bufferNum++)
		if(bufferPool[bufferNum].fileName==fileName&&bufferPool[bufferNum].offsetInFile==offsetInFile){
			flag=1;
			break;
		}
	if(flag==1){
		bufferPool[bufferNum].isValid=0;
		bufferPool[bufferNum].isDirty = 0;
	}
}
//create the block for index
int BufferManager::createNewBlock(string fileName){
	int blockNum=countBlock(fileName);
	return blockNum;//因为是offset，数组下标，所以少1
}