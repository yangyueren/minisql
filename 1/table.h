#ifndef TABLE_H
#define TABLE_H
#include <string>
#include <vector>
#include "Attribute.hpp"
using namespace std;

class Table
{
public:
    Table(string t, vector<Attribute> &attr,int loc):tableName(t),attributeVector(attr),primaryKeyLocation(loc)
    {
        recordNum=0;
        attributeNum=attributeVector.size();
        blockNum=0;
        maxRecordLength=0;
    }
    string tableName;
    int primaryKeyLocation;
    int recordNum;
    int attributeNum;
    int blockNum;
    int maxRecordLength = getRecordLength();
    vector<Attribute> attributeVector;
    string getName()
    {
        return this->tableName;
    }
    int getRecordLength()
    {
        int len=0;
        for(int i=0; i<attributeNum; i++)
        {
            if(attributeVector[i].type == 0)
                len += sizeof(int);
            else if(attributeVector[i].type == -1)
                len += sizeof(float);
            else
                len += (attributeVector[i].type)*(sizeof(char));
        }
        return len;
    }

};

#endif // TABLE_H
