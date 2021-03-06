#include<iostream>
#include"RecordManager.h"
#include<cstring>
#include<cstdio>
#define no_empty 1
#define blockSize 4096
using namespace std;

//create table
int RecordManager::tableCreate(string tableName)
{
	FILE *fp;
	string filename ="TableFile_" + tableName;

	fp = fopen(filename.c_str(),"w+");

	if (fp == NULL) // if fail return 0
		return 0;

	fclose(fp);
	return 1; //operation is success
}

//drop a TABLE 的操作
int RecordManager::tableDrop(string tableName)
{
	string filename = "TableFile_" + tableName;
	bm.setInvalid(filename);
	if(remove(filename.c_str()))  //删除成功remove函数返回0
		return 0;  //删除失败
	return 1;  //删除成功
}


//create index
int RecordManager::indexCreate(string indexName)
{
	FILE *fp;
	string filename = "IndexFile_" + indexName;

	fp = fopen(filename.c_str(),"w+");

	if (fp == NULL) // if fail return 0
		return 0;

	fclose(fp);
	return 1; //operation is success
}

//drop a index 的操作
int RecordManager::indexDrop(string indexName)
{
	string filename = indexName;
	if(remove(filename.c_str()))  //删除成功remove函数返回0
		return 0;  //删除失败
	return 1;  //删除成功
}

int RecordManager::insertRecord(string tableName, char* record, int recordSize)
{
	return 1;
}  //插入记录,返回值是存在第几个块里，如果返回0说明插入失败  
    
    int RecordManager::showClearRecord(string tableName, int recordSize, int bn, vector<Attribute> &attributeVector,vector<Condition> &conditionVector, int in){
    	return 1;
    } //精确查找，in是index返回的块号
   //无索引查找
    int RecordManager::showRecord(string tableName, int recordSize, int bn, vector<Attribute> &attributeVector,vector<Condition> &conditionVector){
    	return 1;
    } //bn是这个表有几个块，输出所有符合条件的记录,返回值是记录数量
    
    //返回记录条数
    int RecordManager::searchRecord(string tableName, int recordSize, int bn, vector<Attribute> &attributeVector,vector<Condition> &conditionVector){
    	return 1;}
    //删除记录
    int RecordManager::deleteRecord(string tableName,int recordSize, int bn,vector<Attribute> &attributeVector,vector<Condition> &conditionVector)
    { return 1;}  //删除符合条件的记录
    //插入索引
    int RecordManager::insertNewIndex(string tableName, int recordSize, int bn, vector<Attribute> &attributeVector,int i ){
    	return 1;
    }
    //更新记录
    int RecordManager::updateRecord(string tabaleName,int recordSize, vector<Condition> &conditionVector){
    	return 1;
    }
/*
//insert a record 插入功能
int RecordManager::insertRecord(string tableName, char* record, int recordSize)
{


	//ask buffer the position
	string filename = "TableFile_" + tableName;
	int pos = bm.getInsertPosition(filename, recordSize + 1);    

	//write record in
	int use = 0;

	while(1)
	{
		if (bm.bufferPool[pos].content[use] != no_empty)
		{
			bm.bufferPool[pos].content[use] = no_empty;
			memcpy(bm.bufferPool[pos].content[use+1], record, recordSize);  //存入record
			break;
		}
		use = use + 1 + recordSize;
	}


	return pos; //insert is successful, 返回块号
}



int RecordManager::showClearRecord(string tableName, int recordSize, int bn, vector<Attribute> &attributeVector,vector<Condition> &conditionVector, int in)  //bn是该table下的block数量，catelog提供
{

	//travel all records if manzu
	string filename ="TableFile_" + tableName;

	int c = 0;
	
	c = c + showBlockRecord(recordSize, in, attributeVector, conditionVector);
		
	return c;
}



//show  all record 查找全部功能，不用condition了，输出全部记录

int RecordManager::showRecord(string tableName, int recordSize, int bn, vector<Attribute> &attributeVector,vector<Condition> &conditionVector)  //bn是该table下的block数量，catelog提供
{
	//先输出属性行
	//travel all records if manzu
	string filename ="TableFile_" + tableName;
	int i = 0;
	int c = 0;
	while(1)   //遍历所有记录
	{
		
		if (i == bn)
			break;
		int s = bm.getBuffer(filename, i);

		c = c + showBlockRecord(recordSize, s, attributeVector, conditionVector);
		
		i = i + 1;
	}
	return c;
}

int RecordManager::showBlockRecord(int recordSize, int num, vector<Attribute> &attributeVector,vector<Condition> &conditionVector)
{
	int use = 0;
	int count = 0;
	while(use < blockSize)
	{
		if(bm.bufferPool[num].content[use] == no_empty)  //if record is available
		{
			if (ifCondition(bm.bufferPool[num].content[use+1], recordSize, attributeVector, conditionVector))
			{
				printRecord(bm.bufferPool[num].content[use+1], recordSize, attributeVector);
				count++;
			}				
		}


		use = use + recordSize + 1;  //go to next record
	}
	return count;
}

/*
int RecordManager::searchRecord(string tableName, int recordSize, int bn, vector<Attribute> &attributeVector, vector<Condition> &conditionVector)  //bn是该table下的block数量，catelog提供
{
	//先输出属性行
	//travel all records if manzu
	string filename ="TableFile_" + tableName;
	int i = 0;
	int c = 0;
	while(1)   //遍历所有记录
	{
		
		if (i == bn)
			break;
		int s = bm.getBuffer(filename, i);

		c = c + searchBlockRecord(recordSize, s, attributeVector, conditionVector);
		
		i = i + 1;
	}
	return c;
}

int RecordManager::searchBlockRecord(int recordSize, int num, vector<Attribute> &attributeVector,vector<Condition> &conditionVector)
{
	int use = 0;
	int count = 0;
	while(use < blockSize)
	{
		if(bm.bufferPool[num].content[use] == no_empty)  //if record is available
		{
			if (ifCondition(use, recordSize, attributeVector, conditionVector))
			{
				count++;
			}				
		}
		
		use = use + recordSize + 1;  //go to next record
	}
	return count;
}




//删除符合条件的记录
int RecordManager::deleteRecord(string tableName,int recordSize, int bn,vector<Attribute> &attributeVector,vector<Condition> &conditionVector)  //bn是该table下的block数量，catelog提供
{
	//先输出属性行
	//travel all records if manzu
	string filename ="TableFile_" + tableName;
	int i = 0;
	while(1)   //遍历所有记录
	{
		
		if (i == bn)
			break;
		int s = bm.getBuffer(filename, i);

		deleteBlockRecord(recordSize, s, attributeVector, conditionVector);
		
		i = i + 1;
	}
	return 1;
}

int RecordManager::deleteBlockRecord(int recordSize, int num, vector<Attribute> &attributeVector,vector<Condition> &conditionVector)
{
	int use = 0;
	while(use < blockSize)
	{
		if(bm.bufferPool[num].content[use] == no_empty)  //if record is available
		{
			if (ifCondition(use, recordSize, attributeVector, conditionVector))
			{
				bm.bufferPool[num].content[use] = empty;
				bm.writtenToDisk(num);
			}
				
		}
		
		use = use + recordSize + 1;  //go to next record
	}
	return 1;
}







*/
//私函数 判断是否满足条件
/*int RecordManager::ifCondition(char* recordBegin, int recordSize, vector<Attribute> &attributeVector,vector<Condition> &conditionVector)
{
    if (conditionVector == NULL) 
        return 1;
    
    int type;
    string attributeName;
    int typeSize;
    
    
    char *contentBegin = recordBegin;
    for(int i = 0; i < attributeVector.size(); i++)
    {
        type = attributeVector[i].type;
        attributeName = attributeVector[i].name;

        if (type == -1)
        {
        	typeSize = sizeof(float);
        	float tmp;
        }
        if(type == 0)
        {
        	typeSize = sizeof(int);
        	int tmp;
        }
        else
        {
        	typeSize = type;
        	string tmp;
        }
        

        memcpy(&tmp, contentBegin, typeSize);

        for(int j = 0; j < conditionVector.size(); j++)
        {
            if (conditionVector[j].attributeName == attributeName)
            {
                if(!conditionVector[j].judege(tmp))
                 	return 0;
            }
        }

        contentBegin += typeSize;
    }
    return 1;
}


//私函数 输出该条语句
void RecordManager::printRecord(char* recordBegin, int recordSize, vector<Attribute> &attributeVector) //print this record
{
	int size = attributeVector.size();
	int i = 0;
	char* p = recordBegin;
	for (int i = 0; i < size; i++)
	{
		if (attributeVector[i].type == -1)  //浮点数
   		{
   			float f ;  
   			memcpy(&f, &p, sizeof(float));
   			cout << f  << "  ";
   			p = p + sizeof(float);
   		}

   		if (attributeVector[i].type == 0)  //int类型
   		{
   			int  d;
   			memcpy(&d, &p, sizeof(int));
   			p = p + sizeof(int);
   			cout << d  << "  ";
   		}

   		else //char类型
   		{
   			string s ;
   			int maxlength = attributeVector[i].type;
   			memcpy(&s, &p, maxlength);  
   			p = p + maxlength;  
   			cout << s << "  ";
   		}

	}

}

/*
bool CreateIndex(string IndexName, int KeySize, int KeyType);//int float   传入一个索引名（实际为文件名）、建索引的值得大小、建索引的值的类型，创建一个空索引，成功返回true，否则返回false
bool CreateIndex(string IndexName, int KeySize, int KeyType, int Degree);// string   传入一个索引名（实际为文件名）、建索引的值得大小、建索引的值的类型、树的degree是char的长度，创建一个空索引，成功返回true，否则返回false     
bool InsertIntoIndex(string indexName, key, keytype, OffsetType Offset)// 
*/
//api
/*	CreateIndex(IndexName, KeySize, KeyType);
else
	CreateIndex(IndexName, KeySize, KeyType, KeySize);
int indexNewCreate(string indexName, int )
{

}*/
/*
int RecordManager::insertNewIndex(string tableName, int recordSize, int bn, vector<Attribute> &attributeVector,int j)  //bn是该table下的block数量，catelog提供
{
	//先输出属性行
	//travel all records if manzu
	string filename ="TableFile_" + tableName;
	int i = 0;
	int c = 0;
	while(1)   //遍历所有记录
	{
		
		if (i == bn)
			break;
		int s = bm.getBuffer(filename, i);

		c = c + insertNewBlockIndexe(recordSize, s, attributeVector, conditionVector);
		
		i = i + 1;
	}
	return c;
}
*/
/*int RecordManager::insertNewBlockIndex(string indexName, int recordSize, int num, vector<Attribute> &attributeVector,vector<Condition> &conditionVector)
{
	int use = 0;
	int count = 0;
	string indexName = "IndexFile_" + indexName;
	while(use < blockSize)
	{
		if(bm.bufferPool[num].content[use] == no_empty)  //if record is available
		{
			char* addressBeigin = bm.bufferPool[num].content[use+1];
			int i = 0;
			int j = 0;
			while(1)
			{
				int length;
				if (attributeVector[i].type == -1)
					length = sizeof(float);
				if (attributeVector[i].type == 0)
					length = sizeof(int);
				else
					length = attributeVector[i].type;

				if (attributeVector[i].name == conditionVector[0].Attribute_Name)
				{
					string key;
					memcpy(&key, addressBeigin, length);
					InsertIntoIndex(indexName, key, conditionVector[0].type, num);	
					break;
				}

				addressBeigin = addressBeigin + length;
			}

			
		}

		use = use + recordSize + 1;  //go to next record
	}
	return count;
}
*/
