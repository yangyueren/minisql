#ifndef RECORDMANAGER_H
#define RECORDMANAGER_H

#include "Condition.hpp"
#include "Attribute.hpp"
#include "RecordManager.h"
#include "BufferManager.h"
#include <string>
#include <vector>
using namespace std;


class RecordManager{
public:
	RecordManager(){};
    BufferManager bm;
    
    int tableCreate(string tableName);  //创建表
    int tableDrop(string tableName);  //删除表   
    int indexCreate(string indexName);  //创建索引
    int indexDrop(string indexName);  //删除索引  

    int insertRecord(string tableName, char* record, int recordSize);  //插入记录,返回值是存在第几个块里，如果返回0说明插入失败  
    

    //索引查找
    int showClearRecord(string tableName, int recordSize, int bn, vector<Attribute> &attributeVector,vector<Condition> &conditionVector, int in); //精确查找，in是index返回的块号
   //无索引查找
    int showRecord(string tableName, int recordSize, int bn, vector<Attribute> &attributeVector,vector<Condition> &conditionVector); //bn是这个表有几个块，输出所有符合条件的记录,返回值是记录数量
    
    //返回记录条数
    int searchRecord(string tableName, int recordSize, int bn, vector<Attribute> &attributeVector,vector<Condition> &conditionVector);
    //删除记录
    int deleteRecord(string tableName,int recordSize, int bn,vector<Attribute> &attributeVector,vector<Condition> &conditionVector);  //删除符合条件的记录
    //插入索引
    int insertNewIndex(string tableName, int recordSize, int bn, vector<Attribute> &attributeVector,vector<Condition> &conditionVector);
    //更新记录
    int updateRecord(string tabaleName,int recordSize, vector<Condition> &conditionVector);

private:


    int showBlockRecord(int recordSize, int num, vector<Attribute> &attributeVector,vector<Condition> &conditionVector);

    int searchBlockRecord(int recordSize, int num, vector<Attribute> &attributeVector,vector<Condition> &conditionVector);
    int deleteBlockRecord(int recordSize, int num, vector<Attribute> &attributeVector,vector<Condition> &conditionVector);
    int ifCondition(char* recordBegin,int recordSize, vector<Attribute> &attributeVector,vector<Condition> &conditionVector);
    void printRecord(char* recordBegin, int recordSize, vector<Attribute> &attributeVector);
    int insertNewBlockIndex(string indexName, int recordSize, int num, vector<Attribute> &attributeVector,vector<Condition> &conditionVector);
};

#endif
