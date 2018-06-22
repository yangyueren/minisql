#include<iostream>
#include"RecordManager.h"
#include<cstring>
using namespace std;

//create table
int RecordManger::tableCreate(string tableName)
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
int RecordManger::tableDrop(string tableName)
{
	string filename = "TableFile_" + tableName;
	if(remove(filename.c_str()))  //删除成功remove函数返回0
		return 0;  //删除失败
	return 1;  //删除成功
}


//create index
int RecordManger::indexCreate(string indexName)
{
	FILE *fp;
	string filename ="IndexFile_" + indexName;

	fp = fopen(filename.c_str(),"w+");

	if (fp == NULL) // if fail return 0
		return 0;

	fclose(fp);
	return 1; //operation is success
}

//drop a index 的操作
int RecordManger::indexDrop(string indexName)
{
	string filename = "IndexFile_" + indexName;
	if(remove(filename.c_str()))  //删除成功remove函数返回0
		return 0;  //删除失败
	return 1;  //删除成功
}



//insert a record 插入功能
int RecordManger::insertRecord(string tableName, char* record, int recordSize)
{


	//ask buffer the position
	pos = bm.getInsertPosition(tableName);    
	if (pos == NULL)
	{
		return -1;
	}
	//write record in

	char* addressBeigin = bm.bufferPool[pos.bufferNum].content[pos.position];
	*addressBeigin = no_empty;  //填入记录是否有效
	addressBeigin ++;
	memcpy(addressBeigin, record, recordSize);  //存入record


	return pos.bufferNum; //insert is successful
}


//
int RecordManger::showClearRecord(string tableName, int recordSize, int bn, vector<Attribute> &attributeVector,vector<Condition> &conditionVector, int in)  //bn是该table下的block数量，catelog提供
{

	//travel all records if manzu
	string filename ="TableFile_" + tableName;
	int s;
	int c = 0;
	
	s = in;

	c = c + showBlockRecord(recordSize, s, attributeVector, conditionVector);
		
	return c;
}



//show  all record 查找全部功能，不用condition了，输出全部记录

int RecordManger::showRecord(string tableName, int recordSize, int bn, vector<Attribute> &attributeVector,vector<Condition> &conditionVector)  //bn是该table下的block数量，catelog提供
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
		s = bm.getBuffer(fileName, i);

		c = c + showBlockRecord(recordSize, s, attributeVector, conditionVector);
		
		i = i + 1;
	}
	return c;
}

int RecordManger::showBlockRecord(int recordSize, int num, vector<Attribute> &attributeVector,vector<Condition> &conditionVector)
{
	int use = 0;
	int count = 0;
	while(use < blocksize)
	{
		if(bm.bufferPool[num].content[use] == no_empty)  //if record is available
		{
			if (ifCondition(use, recordSize, attributeVector, conditionVector))
			{
				printRecord(bm.bufferPool[num].content[use+1], recordSize);
				count++;
			}				
		}
		
	




		use = use + recordSize;  //go to next record
	}
	return count;
}

int RecordManger::searchRecord(string tableName, int recordSize, int bn, vector<Attribute> &attributeVector,vector<Condition> &conditionVector)  //bn是该table下的block数量，catelog提供
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
		s = bm.getBuffer(fileName, i);

		c = c + searchBlockRecord(recordSize, s, attributeVector, conditionVector);
		
		i = i + 1;
	}
	return c;
}

int RecordManger::searchBlockRecord(int recordSize, int num, vector<Attribute> &attributeVector,vector<Condition> &conditionVector)
{
	int use = 0;
	int count = 0;
	while(use < blocksize)
	{
		if(bm.bufferPool[num].content[use] == no_empty)  //if record is available
		{
			if (ifCondition(use, recordSize, attributeVector, conditionVector))
			{
				count++;
			}				
		}
		
		use = use + recordSize;  //go to next record
	}
	return count;
}




//删除符合条件的记录
int RecordManger::deleteRecord(string tableName,int recordSize, int bn,vector<Attribute> &attributeVector,vector<Condition> &conditionVector)  //bn是该table下的block数量，catelog提供
{
	//先输出属性行
	//travel all records if manzu
	string filename ="TableFile_" + tableName;
	int i = 0;
	while(1)   //遍历所有记录
	{
		
		if (i == bn)
			break;
		s = bm.getBuffer(fileName, i);

		deleteBlockRecord(recordSize, s, attributeVector, conditionVector);
		
		i = i + 1;
	}
	return 1;
}

int RecordManger::deleteBlockRecord(int recordSize, int num, vector<Attribute> &attributeVector,vector<Condition> &conditionVector)
{
	int use = 0;
	while(use < blocksize)
	{
		if(bm.bufferPool[num].content[use] == no_empty)  //if record is available
		{
			if (ifCondition(use, recordSize, attributeVector, conditionVector))
			{
				bm.bufferPool[num].content[use] = empty;
				bm.writtenToDisk(num);
			}
				
		}
		
		use = use + recordSize;  //go to next record
	}
	return 1;
}








//私函数 判断是否满足条件
int RecordManager::ifCondition(char* recordBegin,int recordSize, vector<Attribute> &attributeVector,vector<Condition> &conditionVector)
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
        

        memcpy(tmp, contentBegin, typeSize);

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
void printRecord(char* recordBegin, int recordSize, vector<Attribute> &attributeVector) //print this record
{
	int size = attributeVector.size();
	int i = 0;
	p = recordBegin;
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
   			maxlength = attributeVector[i].type;
   			memcpy(&s, &p, i.maxlength);  
   			p = p + i.maxlength;  
   			cout << s << "  ";
   		}

	}

}

















