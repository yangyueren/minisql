/*This is our own CatalogManager.h*/

#ifndef _CATALOGMANAGER_H
#define _CATALOGMANAGER_H

#include <string>
#include <vector>
#include "Attribute.hpp"
#include "IndexInfo.h"
using namespace std;
class CatalogManager{
public:
//    CatalogManager();
//    virtual ~CatalogManager();
    int addIndex(string indexName,string tableName,string attributeName,int type);
    int addTable(string tableName, vector<Attribute> &attributeVector, int primaryKeyLocation );
    int calculateLength(string tableName);
    int calculateLength2(int type);
    int deleteValue(string tableName, int deleteNum);// delete the number of record
    int dropIndex(string indexName);
    int dropTable(string tableName);
    int findIndex(string indexName);
    int findTable(string tableName);
    int getAllIndex(vector<IndexInfo> * indexes);
    int getAttributes( string tableName, vector<Attribute>* attributeVector);
    int getIndexNameList(string tableName, vector<string>* indexNameVector);
    int getIndexType(string indexName);
    int getRecordNum(string tableName);
    void getRecordString(string tableName, vector<string>* recordContent, char* recordResult);
    int insertRecord(string tableName, int recordNum);  // increment the number of record
    int revokeIndexOnAttribute(string tableName,string AttributeName,string indexName);
    int setIndexOnAttribute(string tableName,string AttributeName,string indexName);
private:
	static int IndexNum;
    
};

#endif 
