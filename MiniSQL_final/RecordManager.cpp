#include<iostream>
#include"RecordManager.h"
#include<cstring>
#include<cstdio>
#include<sstream>
#define no_empty 1
#define empty '*'
#define blockSize 4096
using namespace std;
union toyyr
{
    int integer;
    float decimal;
    char character[256];
}tozzy;
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
    string filename = "IndexFile_" +indexName;
    if(remove(filename.c_str()))  //删除成功remove函数返回0
        return 0;  //删除失败
    return 1;  //删除成功
}



//insert a record 插入功能
int RecordManager::insertRecord(string tableName, char* record, int recordSize)
{
    
    
    //ask buffer the position
    string filename = "TableFile_" + tableName;
    int pos = -1;
    int a =  -1;
    pos = bm.getInsertPosition(filename, recordSize + 1);
    
    //cout<<bm.bufferPool[pos].content<<endl;
    //write record in
    int use = 0;
    
    while(1)
    {
        if (bm.bufferPool[pos].content[use] != no_empty)
        {
            bm.bufferPool[pos].content[use] = no_empty;
            memset(&bm.bufferPool[pos].content[use+1], 0, recordSize);
            memcpy(&bm.bufferPool[pos].content[use+1], record, recordSize);  //存入record
            a = bm.bufferPool[pos].offsetInFile;
            bm.setDirty(pos);
            bm.writtenToDisk(pos);
            break;
        }
        use = use + 1 + recordSize;
    }
    
    
    return a; //insert is successful, 返回块号
}




int RecordManager::showClearRecord(string tableName, int recordSize, int bn, vector<Attribute> &attributeVector,vector<Condition> &conditionVector, int in)  //bn是该table下的block数量，catelog提供
{
    string filename ="TableFile_" + tableName;
    int s = bm.getBuffer(filename, in);
    
    int c = 0;
    c = showBlockRecord(recordSize, s, attributeVector, conditionVector);
    
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
            if (ifCondition(&bm.bufferPool[num].content[use+1], recordSize, attributeVector, conditionVector))
            {
                printRecord(&bm.bufferPool[num].content[use+1], recordSize, attributeVector);
                count++;
            }
        }
        
        use = use + recordSize + 1;  //go to next record
    }
    return count;
}

int RecordManager::searchClearRecord(string tableName, int recordSize, int bn, vector<Attribute> &attributeVector,vector<Condition> &conditionVector, int in)  //bn是该table下的block数量，catelog提供
{
    string filename ="TableFile_" + tableName;
    int s = bm.getBuffer(filename, in);
    
    int c = 0;
    c = c + searchBlockRecord(recordSize, s, attributeVector, conditionVector);
    
    return c;
}

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
            if (ifCondition(&bm.bufferPool[num].content[use+1], recordSize, attributeVector, conditionVector))
            {
                count++;
            }
        }
        
        use = use + recordSize + 1;  //go to next record
    }
    return count;
}


//有索引删除

int RecordManager::deleteClearRecord(string tableName, int recordSize, int bn, vector<Attribute> &attributeVector,vector<Condition> &conditionVector, int in)  //bn是该table下的block数量，catelog提供
{
    
    string filename ="TableFile_" + tableName;
    int s = bm.getBuffer(filename, in);
    
    int c = 0;
    c = c + deleteBlockRecord(recordSize, s, attributeVector, conditionVector);
    
    return c;
}

//删除符合条件的记录
int RecordManager::deleteRecord(string tableName,int recordSize, int bn,vector<Attribute> &attributeVector,vector<Condition> &conditionVector)  //bn是该table下的block数量，catelog提供
{
    //先输出属性行
    //travel all records if manzu
    int c = 0;
    string filename ="TableFile_" + tableName;
    int i = 0;
    while(1)   //遍历所有记录
    {
        
        if (i == bn)
            break;
        int s = bm.getBuffer(filename, i);
        
        c = c + deleteBlockRecord(recordSize, s, attributeVector, conditionVector);
        
        i = i + 1;
    }
    return c;
}
/*
int RecordManager::deleteBlockRecord(int recordSize, int num, vector<Attribute> &attributeVector,vector<Condition> &conditionVector)
{
    int use = 0;
    
    int count = 0;
    while(use < blockSize)
    {
        if(bm.bufferPool[num].content[use] == no_empty)  //if record is available
        {
            if (ifCondition(&bm.bufferPool[num].content[use+1], recordSize, attributeVector, conditionVector))
            {
                bm.bufferPool[num].content[use] = empty;
                char * begin = &bm.bufferPool[num].content[use + 1];
                
                for(int i = 0; i < attributeVector.size(); i++)
                {
                    int asone = attributeVector[i].type;
                    int Key_Type = -1;
                    string KeyValue;
                    if(asone == -1)
                    {
                        Key_Type = 1;
                        float f;
                        memcpy(&f, begin, sizeof(float));
                        stringstream ss(f);
                        ss >> KeyValue;
                        begin = begin + sizeof(float);
                    }
                    
                    else if(asone == 0)
                    {
                        Key_Type = 0;
                        int d;
                        memcpy(&d, begin, sizeof(int));
                        stringstream ss(d);
                        ss >> KeyValue;
                        begin = begin + sizeof(int);
                    }
                    else
                    {
                        Key_Type = 2;
                        char po[attributeVector[i].type];
                        memset(po, 0, attributeVector[i].type);
                        memcpy(po, begin, attributeVector[i].type);
                        KeyValue = po;
                        begin = begin + attributeVector[i].type;
                    }
                    if(attributeVector[i].index != "")
                    {
                        string IndexName = "IndexFile_" + attributeVector[i].index;
                        index_manager.DeleteInIndex(IndexName, KeyValue, Key_Type);
                    }
                    i++;
                    
                }
                
                count ++;
            }
            
        }
        
        use = use + recordSize + 1;  //go to next record
    }
    bm.setDirty(num);
    bm.writtenToDisk(num);
    return count;
}*/
int RecordManager::deleteBlockRecord(int recordSize, int num, vector<Attribute> &attributeVector,vector<Condition> &conditionVector)
{
    int use = 0;
    int count = 0;
    while(use < blockSize)
    {
        if(bm.bufferPool[num].content[use] == no_empty)  //if record is available
        {
            if (ifCondition(&bm.bufferPool[num].content[use+1], recordSize, attributeVector, conditionVector))
            {
                bm.bufferPool[num].content[use] = empty;
                //需要删除一下索引
                count ++;
            }
            
        }
        
        use = use + recordSize + 1;  //go to next record
    }
    bm.setDirty(num);
    bm.writtenToDisk(num);
    return count;
}

//私函数 判断是否满足条件
int RecordManager::ifCondition(char* recordBegin, int recordSize, vector<Attribute> &attributeVector,vector<Condition> &conditionVector)
{
    if (conditionVector.size() == 0)
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
            memcpy(&tmp, contentBegin, typeSize);
            for(int j = 0; j < conditionVector.size(); j++)
            {
                if (conditionVector[j].Attribute_Name == attributeName)
                {
                    if(!conditionVector[j].judge(tmp))
                        return 0;
                }
            }
        }
        
        else if(type == 0)
        {
            typeSize = sizeof(int);
            int tmp;
            memcpy(&tmp, contentBegin, typeSize);
            for(int j = 0; j < conditionVector.size(); j++)
            {
                if (conditionVector[j].Attribute_Name == attributeName)
                {
                    if(!conditionVector[j].judge(tmp))
                        return 0;
                }
            }
        }
        else
        {
            typeSize = type;
            char a[type];
            memset(a, 0, type);
            memcpy(a, contentBegin, typeSize);
            string tmp = a;
            for(int j = 0; j < conditionVector.size(); j++)
            {
                if (conditionVector[j].Attribute_Name == attributeName)
                {
                    if(!conditionVector[j].judge(tmp))
                        return 0;
                }
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
    char* p = recordBegin;
    for (int i = 0; i < size; i++)
    {
        if (attributeVector[i].type == -1)  //浮点数
        {
            float f ;
            memcpy(&f, p, sizeof(float));
            p = p + sizeof(float);
            cout << f;
            if(i != size - 1)
                cout  << " , ";
        }
        
        else if (attributeVector[i].type == 0)  //int类型
        {
            int  d;
            memcpy(&d, p, sizeof(int));
            p = p + sizeof(int);
            cout << d ;
            if(i != size - 1)
                cout<< " , ";
        }
        
        else //char类型
        {
            int maxlength = attributeVector[i].type;
            char asd[256];
            memset(asd, 0, 256);
            memcpy(asd, p, maxlength);
            /*              for (int nice = 0; nice < maxlength; nice++)
             {
             if(asd[nice] == '*')
             asd[nice] = '0';
             }*/
            string s = asd;
            p = p + maxlength;
            cout << s ;
            if(i != size - 1)
                cout << " , ";
        }
        
    }
    cout << endl;
    
}


//UPDATE
int RecordManager::updateClearRecord(string tableName, int recordSize, vector<Attribute> &attributeVector,vector<Condition> &conditionVector, int in)
{
    string filename ="TableFile_" + tableName;
    
    
    int s = bm.getBuffer(filename, in);
    
    int c = 0;
    c = c + updateBlockRecord(recordSize, s, attributeVector, conditionVector);
    
    return c;
}

int RecordManager::updateRecord(string tableName, int recordSize, int bn, vector<Attribute> &attributeVector,vector<Condition> &conditionVector)
{
    string filename ="TableFile_" + tableName;
    int i = 0;
    int c = 0;
    while(1)   //遍历所有记录
    {
        
        if (i == bn)
            break;
        int s = bm.getBuffer(filename, i);
        
        c = c + updateBlockRecord(recordSize, s, attributeVector, conditionVector);
        
        i = i + 1;
    }
    return c;
}

int RecordManager::updateBlockRecord(int recordSize, int num, vector<Attribute> &attributeVector,vector<Condition> &conditionVector)
{
    int use = 0;
    int count = 0;
    while(use < blockSize)
    {
        if(bm.bufferPool[num].content[use] == no_empty)  //if record is available
        {
            if (ifUpdateCondition(&bm.bufferPool[num].content[use+1], recordSize, attributeVector, conditionVector))
            {
                int t = 0;
                for (int i = 0; i < attributeVector.size(); i++)
                {
                    int typeSize;
                    if (attributeVector[i].type == -1)
                    {
                        typeSize = sizeof(float);
                        if (conditionVector[0].Attribute_Name == attributeVector[i].name)
                        {
                            stringstream ss(conditionVector[0].value);
                            float f;
                            ss >> f;
                            memcpy(&bm.bufferPool[num].content[use+1+t], &f, sizeof(float) );
                        }
                    }
                    
                    if (attributeVector[i].type == 0)
                    {
                        typeSize = sizeof(int);
                        if (conditionVector[0].Attribute_Name == attributeVector[i].name)
                        {
                            stringstream ss(conditionVector[0].value);
                            int d;
                            ss >> d;
                            memcpy(&bm.bufferPool[num].content[use+1+t], &d, sizeof(int));
                        }
                    }
                    
                    else
                    {
                        typeSize = attributeVector[i].type;
                        if (conditionVector[0].Attribute_Name == attributeVector[i].name)
                        {
                            char assone[typeSize];
                            memset(assone, 0, typeSize);
                            memcpy(assone, &conditionVector[0].value, typeSize);
                            memcpy(&bm.bufferPool[num].content[use+1+t], assone, typeSize);
                        }
                    }
                    
                    t = t + typeSize;
                }
                
                
                
                count ++;
            }
            
        }
        
        use = use + recordSize + 1;  //go to next record
    }
    bm.setDirty(num);
    bm.writtenToDisk(num);
    return count;
}

//私函数 判断是否满足条件
int RecordManager::ifUpdateCondition(char* recordBegin, int recordSize, vector<Attribute> &attributeVector,vector<Condition> &conditionVector)
{
    if (conditionVector.size() == 1)
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
            memcpy(&tmp, contentBegin, typeSize);
            for(int j = 1; j < conditionVector.size(); j++)
            {
                if (conditionVector[j].Attribute_Name == attributeName)
                {
                    if(!conditionVector[j].judge(tmp))
                        return 0;
                }
            }
        }
        
        else if(type == 0)
        {
            typeSize = sizeof(int);
            int tmp;
            memcpy(&tmp, contentBegin, typeSize);
            for(int j = 1; j < conditionVector.size(); j++)
            {
                if (conditionVector[j].Attribute_Name == attributeName)
                {
                    if(!conditionVector[j].judge(tmp))
                        return 0;
                }
            }
        }
        else
        {
            typeSize = type;
            char a[type];
            memset(a, 0, type);
            memcpy(a, contentBegin, typeSize);
            string tmp = a;
            for(int j = 1; j < conditionVector.size(); j++)
            {
                if (conditionVector[j].Attribute_Name == attributeName)
                {
                    if(!conditionVector[j].judge(tmp))
                        return 0;
                }
            }
        }
        
        
        contentBegin += typeSize;
    }
    return 1;
}

/*
 bool CreateIndex(string IndexName, int KeySize, int KeyType);//int float   传入一个索引名（实际为文件名）、建索引的值得大小、建索引的值的类型，创建一个空索引，成功返回true，否则返回false
 bool CreateIndex(string IndexName, int KeySize, int KeyType, int Degree);// string   传入一个索引名（实际为文件名）、建索引的值得大小、建索引的值的类型、树的degree是char的长度，创建一个空索引，成功返回true，否则返回false
 bool InsertIntoIndex(string indexName, key, keytype, OffsetType Offset)//
 */
//api
/*    CreateIndex(IndexName, KeySize, KeyType);
 else
 CreateIndex(IndexName, KeySize, KeyType, KeySize);
 int indexNewCreate(string indexName, int )
 {
 
 }*/



int RecordManager::insertNewIndex(string tableName, string indexName, int recordSize, int bn, vector<Attribute> &attributeVector,int j)  //bn是该table下的block数量，catelog提供
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
        
        c = c + insertNewBlockIndex(indexName, recordSize, s, attributeVector, j);
        
        i = i + 1;
    }
    return c;
}


int RecordManager::insertNewBlockIndex(string indexName, int recordSize, int num, vector<Attribute> &attributeVector, int j)
{
    int use = 0;
    int count = 0;
    string fileindexName = "IndexFile_" + indexName;
    while(use < blockSize)
    {
        if(bm.bufferPool[num].content[use] == no_empty)  //if record is available
        {
            char* addressBeigin = &bm.bufferPool[num].content[use+1];
            int i = 0;
            while(1)
            {
                int length;
                int a;
                float b;
                string c;
                string www;
                if (attributeVector[i].type == -1){
                    length = sizeof(float);
                    memset(tozzy.character, 0, 100);
                    memcpy(tozzy.character, addressBeigin, length);
                    b = tozzy.decimal;
                    www = to_string(b);
                }
                
                else if (attributeVector[i].type == 0){
                    length = sizeof(int);
                    memset(tozzy.character, 0, 100);
                    memcpy(tozzy.character, addressBeigin, length);
                    a = tozzy.integer;
                    www = to_string(a);
                }
                
                else{
                    length = attributeVector[i].type;
                    memset(tozzy.character, 0, 100);
                    memcpy(tozzy.character, addressBeigin, length);
                    c = tozzy.character;
                    www = c;
                }
                
                if (i == j)
                {
                    /*
                    char key[100];
                    memset(key, 0, 100);
                    memcpy(key, addressBeigin, length);
                    string www(key);*/
                    int syy;
                    if(attributeVector[i].type > 0)
                        syy = 2;
                    else if (attributeVector[i].type == 0)
                        syy = 0;
                    else
                        syy = 1;
                    index_manager.InsertIntoIndex(fileindexName, www, syy, bm.bufferPool[num].offsetInFile);
                    count++;
                    break;
                }
                i++;
                addressBeigin = addressBeigin + length;
            }
            
        }
        
        use = use + recordSize + 1;  //go to next record
    }
    return count;
}



