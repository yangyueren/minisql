/*This is our own CatalogManager.cpp*/

#include "CatalogManager.h"
#include "table.h"
#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>

#define UNKNOWN_FILE 8
#define TABLE_FILE 9
#define INDEX_FILE 10
#define MAXNAMELENGTH 256
#define MAXLINELENGTH 2000

// int main()
// {
//     CatalogManager a;
//     Attribute titit("kkk",1,false,"jkl");
//     Attribute t2("jjd",1,true,"");
//     vector<Attribute> v1;
//     vector<string> *inv1 = new vector<string>;
//     v1.push_back(titit);
//     v1.push_back(t2);
//     a.addTable("Table1",v1,1);
//     a.addIndex("aab","Table1","ccccc",1);
//     a.addIndex("aaa","Table1","ccccd",1);
//     a.addIndex("ccc","Table1","ccccdd",1);
//     a.addIndex("abcde","Table1","ccccddd",-1);
//     a.addIndex("a345678","Table1","kkk",1);
//     //a.insertRecord("Table1",9);
//     cout<<a.findIndex("aaa")<<endl;
//     cout<<a.findTable("Table1")<<endl;
//     cout<<a.findIndex("aza")<<endl;
//     cout<<a.findTable("Tab")<<endl;
//     a.insertRecord("Table1",9);
//     //a.dropIndex("a345678");
//     //a.revokeIndexOnAttribute("Table1","kkk","a345678");
//     //a.deleteValue("Table1",1);
//     //a.getIndexNameList("n23n",inv1);
//     //cout<<a.getIndexType("abcde")<<endl;
//     //a.dropTable("Table1");
//     /*FILE *fp;
//     fp=fopen("indexes.txt","r");
//     char tempStr[100]={0};
//     while(1)
//     {
//         if(feof(fp)) break;
//         fgets(tempStr,sizeof(tempStr),fp);
//         printf("%s ###\n",tempStr);
//     }*/
//     return 0;
// }
int CatalogManager::addIndex(string indexName, string tableName, string attributeName, int type)
{
    int curnum;
    string filename = "indexes.txt";
    FILE *fp;
    fp = fopen(filename.c_str(),"r+");
    IndexInfo tempIndex(indexName,tableName,attributeName,type);
    fseek(fp,0,SEEK_SET);
    fscanf(fp,"%d",&curnum);
    curnum++;
    fseek(fp, 0, SEEK_SET);
    fprintf(fp, "%d\n", curnum);
    fseek(fp, 0, SEEK_END);
    fprintf(fp, "%s %s %s %d\n", tempIndex.indexName.c_str(), tempIndex.tableName.c_str(), tempIndex.attributeName.c_str(), tempIndex.type);
    fclose(fp);
    return this->setIndexOnAttribute(tableName, attributeName, indexName);
}

int CatalogManager::addTable(string tableName, vector<Attribute> &attributeVector, int primaryKeyLocation)
{
    FILE *fp;
    string fname = tableName + ".txt";
    char tindex[MAXNAMELENGTH];
    memset(tindex,' ',MAXNAMELENGTH-1);
    tindex[MAXNAMELENGTH-1]='\0';
    fp = fopen(fname.c_str(),"w+");
    if (fp == NULL)
    {
        return 0;
    }
    Table t1(tableName,attributeVector,primaryKeyLocation);
    fprintf(fp,"%s\n%d\n%d\n%d\n",t1.tableName.c_str(),t1.primaryKeyLocation,t1.recordNum,t1.attributeNum);
    for(int i = 0; i<t1.attributeNum; i++)
    {
        fprintf(fp,"%s %d %d ",attributeVector[i].name.c_str(),attributeVector[i].type,attributeVector[i].unique);
        fputs(tindex,fp);
        fprintf(fp,"\n");
    }
    fclose(fp);
    return 1;
}

int CatalogManager::calculateLength(string tableName)
{
    int attrnum,ttype,tuniq,count=0;
    char temp[MAXLINELENGTH];
    string fname = tableName + ".txt";
    FILE *fp;
    fp = fopen(fname.c_str(),"r");
    fseek(fp,0,SEEK_SET);
    for(int i=0; i<3; i++)
        fgets(temp,sizeof(temp),fp);
    fscanf(fp,"%d",&attrnum);
    for(int i=0 ;i<attrnum; i++)
    {
        fscanf(fp,"%s",temp);
        fscanf(fp,"%d",&ttype);
        fscanf(fp,"%d",&tuniq);
        fscanf(fp,"%s",temp);
        count+=calculateLength2(ttype);
    }
    return count;
}

int CatalogManager::calculateLength2(int type)
{
	const int temp = type;

    if (type == 0) {
        return sizeof(int);
    }
    else if (type == -1)
    {
        return sizeof(float);
    }
    else{
        return (int)sizeof(char) * type; // Note that the type stores in Attribute.h
    }
}

int CatalogManager::deleteValue(string tableName, int deleteNum)
{
    char temp[MAXLINELENGTH];
    int recordNum;
    string fname = tableName + ".txt";
    FILE *fp;
    fp = fopen(fname.c_str(),"r+");
    fseek(fp,0,SEEK_SET);
    fgets(temp,sizeof(temp),fp);
    fgets(temp,sizeof(temp),fp);
    fscanf(fp,"%d",&recordNum);
    //printf("%d\n",recordNum);
    if(recordNum >= deleteNum)
    {
        recordNum-=deleteNum;
        fseek(fp, (-1)*(long)sizeof(int), SEEK_CUR);
        fprintf(fp,"\n%d\n",recordNum);
        fclose(fp);
        return 1;
    }
    else
    {
        printf("Error: There are not enough records.\n");
        fclose(fp);
        return 0;
    }
}

int CatalogManager::dropIndex(string indexName)
{
    int num,curnum,f=0,type;
    string fname = "indexes.txt";
    char ttable[MAXNAMELENGTH],tattr[MAXNAMELENGTH],tindex[MAXNAMELENGTH];
    char temp[MAXLINELENGTH];
    FILE *fp;
    fp = fopen(fname.c_str(),"r+");
    fseek(fp,0,SEEK_SET);
    fscanf(fp,"%d\n",&num);
    curnum = num;
    fpos_t tmp;
    //printf("%d\n",num);
    for(int i=0; i<curnum; i++)
    {
        fpos_t flag;
        fgetpos(fp,&flag);
        fgets(temp,MAXLINELENGTH,fp);
        memset(temp,' ',strlen(temp));
        fsetpos(fp,&flag);
        fscanf(fp,"%s",tindex);
        //printf("%s\n",tindex);
        if(!strcmp(tindex,indexName.c_str()))
        {
            fscanf(fp,"%s",ttable);
            fscanf(fp,"%s",tattr);
            fscanf(fp,"%d",&type);
            fgetpos(fp,&tmp);
            this->revokeIndexOnAttribute(ttable,tattr,indexName);
            fsetpos(fp,&flag);
            fputs(temp,fp);
            fprintf(fp,"\n");
            num--;
            fseek(fp, 0, SEEK_SET);
            fprintf(fp, "%d", num);
            f=1;
            fsetpos(fp,&tmp);
        }
        else
        {
            fgets(temp,sizeof(temp),fp);//Deal with the extra information in the same line.
        }
    }
    fclose(fp);
    return f;
}

int CatalogManager::dropTable(string tableName)
{
    string fname = tableName + ".txt";
    if(remove(fname.c_str()))
    {
        return 0;
    }
    return 1;
}

int CatalogManager::findIndex(string indexName)
{
    int num, flag=UNKNOWN_FILE;
    string fname = "indexes.txt";
    char tindex[MAXNAMELENGTH];
    char temp[MAXLINELENGTH];
    FILE *fp;
    fp = fopen(fname.c_str(),"r+");
    fseek(fp,0,SEEK_SET);
    fscanf(fp,"%d\n",&num);
    for(int i=0; i<num; i++)
    {
        fscanf(fp,"%s",tindex);
        if(!strcmp(tindex,indexName.c_str()))
        {
            flag = INDEX_FILE;
            break;
        }
        else
        {
            fgets(temp,sizeof(temp),fp);//Deal with the extra information in the same line.
        }
    }
    return flag;
}

int CatalogManager::findTable(string tableName)
{
    string fname = tableName + ".txt";
    FILE *fp;
    fp = fopen(fname.c_str(), "r");
    if (fp == NULL)
    {
        return 0;
    }
    else
    {
        fclose(fp);
        return TABLE_FILE;
    }
}

int CatalogManager::getAllIndex(vector<IndexInfo> *indexes)
{
    int num,ttype;
    string fname = "indexes.txt";
    char tindex[MAXNAMELENGTH],ttable[MAXNAMELENGTH],tattr[MAXNAMELENGTH];
    FILE *fp;
    fp = fopen(fname.c_str(),"r");
    fseek(fp,0,SEEK_SET);
    fscanf(fp,"%d",&num);
    for(int i=0; i<num; i++)
    {
        fscanf(fp,"%s",tindex);
        fscanf(fp,"%s",ttable);
        fscanf(fp,"%s",tattr);
        fscanf(fp,"%d",&ttype);
        IndexInfo curindex(tindex,ttable,tattr,ttype);
        indexes->push_back(curindex);
    }
    return num;
}

int CatalogManager::getAttributes(string tableName, vector<Attribute> *attributeVector)
{
    int attrnum, ttype, tuniq;
    char temp[MAXLINELENGTH];
    char tattr[MAXNAMELENGTH],tindex[MAXNAMELENGTH];
    memset(tindex,' ',MAXNAMELENGTH-1);
    tindex[MAXNAMELENGTH-1]='\0';
    string fname = tableName + ".txt";
    FILE *fp;
    fp = fopen(fname.c_str(),"r");
    fseek(fp,0,SEEK_SET);
    for(int i=0; i<3; i++)
        fgets(temp,sizeof(temp),fp);
    fscanf(fp,"%d",&attrnum);
    for(int i=0 ;i<attrnum; i++)
    {
        fscanf(fp,"%s",tattr);
        fscanf(fp,"%d",&ttype);
        fscanf(fp,"%d",&tuniq);
        fscanf(fp,"%s",tindex);
        Attribute curattr(tattr,ttype,tuniq,tindex);
        attributeVector->push_back(curattr);
    }
    return attrnum;
}

int CatalogManager::getIndexNameList(string tableName, vector<string> *indexNameVector)
{
    int num;
    string filename = "indexes.txt";
    char tindex[MAXNAMELENGTH],ttable[MAXNAMELENGTH];
    char rub[MAXLINELENGTH];
    FILE *fp;
    fp = fopen(filename.c_str(),"r");
    fseek(fp,0,SEEK_SET);
    fscanf(fp,"%d",&num);
    for(int i=0; i<num; i++)
    {
        fscanf(fp,"%s",tindex);
        fscanf(fp,"%s",ttable);
        fgets(rub,sizeof(rub),fp);//Deal with the extra information in the same line.
        if(!strcmp(ttable,tableName.c_str()))
            indexNameVector->push_back(tindex);
    }
    /*
    for(int i=0; i<indexNameVector->size();i++)
    {
        printf("%s\n",(*indexNameVector)[i].c_str());
    }
    */
    fclose(fp);
    return indexNameVector->size();
}

int CatalogManager::getIndexType(string indexName)
{
    int num,type;
    string filename = "indexes.txt";
    char tindex[MAXNAMELENGTH];
    char temp[MAXLINELENGTH];
    FILE *fp;
    fp = fopen(filename.c_str(),"r");
    fseek(fp,0,SEEK_SET);
    fscanf(fp,"%d",&num);
    for(int i=0; i<num; i++)
    {
        fscanf(fp,"%s",tindex);
        if(!strcmp(tindex,indexName.c_str()))
        {
            fscanf(fp,"%s",temp);
            fscanf(fp,"%s",temp);
            fscanf(fp,"%d",&type);
            fclose(fp);
            return type;
        }
        else
        {
            fgets(temp,sizeof(temp),fp);//Deal with the extra information in the same line.
        }
    }
    return -2;
}

int CatalogManager::getRecordNum(string tableName)
{
    int recordNum;
    char temp[MAXLINELENGTH];
    string fname = tableName + ".txt";
    FILE *fp;
    fp = fopen(fname.c_str(),"r");
    fseek(fp,0,SEEK_SET);
    fgets(temp,sizeof(temp),fp);
    fgets(temp,sizeof(temp),fp);
    fscanf(fp,"%d",&recordNum);
    return recordNum;
}

void CatalogManager::getRecordString(string tableName, vector<string> *recordContent, char *recordResult)
{
    vector<Attribute> attributeVector;
    getAttributes(tableName, &attributeVector);
    char * contentBegin = recordResult;
    for(int i = 0; i < attributeVector.size(); i++)
    {
        Attribute attribute = attributeVector[i];
        string content = (*recordContent)[i];
        int type = attribute.type;
        int typeSize = calculateLength2(type);
        stringstream ss;
        ss << content;
        if (type == 0)
        {
            //if the content is a int
            int intTmp;
            ss >> intTmp;
            memcpy(contentBegin, ((char*)&intTmp), typeSize);
        }
        else if (type == -1)
        {
            //if the content is a float
            float floatTmp;
            ss >> floatTmp;
            memcpy(contentBegin, ((char*)&floatTmp), typeSize);
        }
        else
        {
            //if the content is a string
            memcpy(contentBegin, content.c_str(), typeSize);
        }
        contentBegin += typeSize;
    }
    return ;
}

int CatalogManager::insertRecord(string tableName, int recordNum)
{
    int curRecordNum;
    char temp[MAXLINELENGTH];
    string fname = tableName + ".txt";
    FILE *fp;
    fp = fopen(fname.c_str(),"r+");
    fseek(fp,0,SEEK_SET);
    fgets(temp,sizeof(temp),fp);
    fgets(temp,sizeof(temp),fp);
    fscanf(fp,"%d",&curRecordNum);
    curRecordNum+=recordNum;
    fseek(fp,0,SEEK_CUR);
    fseek(fp, (-1)*(long)(sizeof(int)-sizeof(char)), SEEK_CUR);
    //fwrite(&curRecordNum,sizeof(int),1,fp);
    fprintf(fp,"\n%d\n",curRecordNum);
    fclose(fp);
    return 1;
}

int CatalogManager::revokeIndexOnAttribute(string tableName, string AttributeName, string indexName)
{
    int attrnum,tmp,i;
    char temp[MAXLINELENGTH];
    char tattr[MAXNAMELENGTH],tindex[MAXNAMELENGTH];
    string fname = tableName + ".txt";
    FILE *fp;
    fp = fopen(fname.c_str(),"r+");
    fseek(fp,0,SEEK_SET);
    for(int i=0; i<3; i++)
        fgets(temp,sizeof(temp),fp);
    fscanf(fp,"%d",&attrnum);
    for(i=0 ;i<attrnum; i++)
    {
        fscanf(fp,"%s",tattr);
        if(!strcmp(tattr,AttributeName.c_str()))
        {
            fscanf(fp,"%d",&tmp);
            fscanf(fp,"%d",&tmp);
            fscanf(fp,"%s",tindex);
            //printf("### %s ###\n\n",tindex);
            if(!strcmp(tindex,indexName.c_str()))
            {
                //printf("###I am here.###\n");
                memset(temp,' ',MAXNAMELENGTH-1);
                temp[MAXNAMELENGTH-1]='\0';
                fseek(fp,0,SEEK_CUR);
                fseek(fp,(long)(-1)*indexName.length()*sizeof(char),SEEK_CUR);
                //fprintf(fp,"aaabbbccc\n");
                fputs(temp,fp);
                fprintf(fp,"\n");
                break;
            }
            else
            {
                fclose(fp);
                return 0;
            }
        }
        else
        {
            fgets(temp,sizeof(temp),fp);//Deal with the extra information in the same line.
        }
    }
    fclose(fp);
    return (i<attrnum);
}

int CatalogManager::setIndexOnAttribute(string tableName, string AttributeName, string indexName)
{
    int attrnum,tmp,i;
    char temp[MAXLINELENGTH];
    char tattr[MAXNAMELENGTH];
    char tindex[MAXNAMELENGTH];
    strcpy(tindex,indexName.c_str());
    memset(tindex+indexName.length(),' ',MAXNAMELENGTH-indexName.length()-1);
    tindex[MAXNAMELENGTH-1]='\0';
    string fname = tableName + ".txt";
    FILE *fp;
    fp = fopen(fname.c_str(),"r+");
    fseek(fp,0,SEEK_SET);
    for(int i=0; i<3; i++)
        fgets(temp,sizeof(temp),fp);
    fscanf(fp,"%d",&attrnum);
    for(i=0 ; i<attrnum; i++)
    {
        fscanf(fp,"%s",tattr);
        if(!strcmp(tattr,AttributeName.c_str()))
        {
            //printf("\nYESYESYES\n");
            fscanf(fp,"%d",&tmp);
            fscanf(fp,"%d",&tmp);
            fprintf(fp," ");
            //printf("%s\n",indexName.c_str());
            fseek(fp,0,SEEK_CUR);
            fputs(tindex,fp);
            //printf("%d##%s\n",sizeof(tindex),tindex);
            //fprintf(fp,"\n");
            break;
        }
        else
        {
            //printf("\nNoNoNo\n");
            fgets(temp,sizeof(temp),fp);//Deal with the extra information in the same line.
        }
    }
    fclose(fp);
    return (i<attrnum);
}
