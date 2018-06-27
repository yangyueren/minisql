/*This is our own CatalogManager.cpp*/

#include "CatalogManager.h"
#include "table.h"
#include <cstdio>
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>

#define UNKNOWN_FILE 8
#define TABLE_FILE 9
#define INDEX_FILE 10
#define MAXINTLENGTH 12
#define MAXNAMELENGTH 256
#define MAXLINELENGTH 2000


int CatalogManager::addIndex(string indexName, string tableName, string attributeName, int type)
{
    int curnum;
    char tint[MAXINTLENGTH],inttemp[MAXINTLENGTH];
    string filename = "indexes";
    IndexInfo tempIndex(indexName,tableName,attributeName,type);
    FILE *fp;
    fp = fopen(filename.c_str(),"r+");
    fseek(fp,0,SEEK_SET);
    fscanf(fp,"%s",inttemp);
    curnum=atoi(inttemp);
    curnum++;
    string tmp;
    tmp = to_string(curnum);
    memcpy(tint, tmp.c_str(), sizeof(tmp.c_str()));
    memset(tint+strlen(tint),' ',MAXINTLENGTH-strlen(tint)-1);
    tint[MAXINTLENGTH-1]='\0';
    fseek(fp, 0, SEEK_SET);
    fputs(tint,fp);
    fprintf(fp,"\n");
    fseek(fp, 0, SEEK_END);
    fprintf(fp, "%s %s %s %d\n", tempIndex.indexName.c_str(), tempIndex.tableName.c_str(), tempIndex.attributeName.c_str(), tempIndex.type);
    fclose(fp);
    return 1;
}

int CatalogManager::addTable(string tableName, vector<Attribute> &attributeVector, int primaryKeyLocation)
{
    FILE *fp,*fptest;
    string fname = tableName;
    char tindex[MAXNAMELENGTH];
    char tint[MAXINTLENGTH];
    memset(tindex,' ',MAXNAMELENGTH-1);
    tint[0]='0';
    memset(tint+1,' ',MAXINTLENGTH-2);
    tindex[MAXNAMELENGTH-1]='\0';
    tint[MAXINTLENGTH-1]='\0';
    fptest = fopen("indexes","r");
    if(!fptest)
    {
        fptest = fopen("indexes","w+");
        fputs(tint,fptest);
        fprintf(fptest,"\n");
        fclose(fptest);
    }
    fp = fopen(fname.c_str(),"w+");
    if (fp == NULL)
    {
        return 0;
    }
    Table t1(tableName,attributeVector,primaryKeyLocation);
    fprintf(fp,"%s\n%d\n",t1.tableName.c_str(),t1.primaryKeyLocation);
    fputs(tint,fp);
    fprintf(fp,"\n%d\n",t1.attributeNum);
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
    int sum=0;
    vector<Attribute> v;
    this->getAttributes(tableName,&v);
    for(int i=0; i<v.size(); i++)
    {
        sum += calculateLength2(v[i].type);
    }
    return sum;
}

int CatalogManager::calculateLength2(int type)
{
    const int temp = type;
    
    if (type == 0)
    {
        return sizeof(int);
    }
    else if (type == -1)
    {
        return sizeof(float);
    }
    else
    {
        return (int)sizeof(char) * type; // Note that the type stores in Attribute.h
    }
}

//int CatalogManager::deleteValue(string tableName, int deleteNum)
//{
//    char temp[MAXLINELENGTH];
//    int recordNum;
//    string fname = tableName;
//    FILE *fp;
//    fp = fopen(fname.c_str(),"r+");
//    fseek(fp,0,SEEK_SET);
//    fgets(temp,sizeof(temp),fp);
//    fgets(temp,sizeof(temp),fp);
//    fscanf(fp,"%d",&recordNum);
//    //printf("%d\n",recordNum);
//    if(recordNum >= deleteNum)
//    {
//        recordNum-=deleteNum;
//        fseek(fp, (-1)*(long)sizeof(int), SEEK_CUR);
//        fprintf(fp,"\n%d\n",recordNum);
//        fclose(fp);
//        return 1;
//    }
//    else
//    {
//        printf("Error: There are not enough records.\n");
//        fclose(fp);
//        return 0;
//    }
//}

int CatalogManager::dropIndex(string indexName)
{
    int num,curnum,f=0,type;
    string fname = "indexes";
    char ttable[MAXNAMELENGTH],tattr[MAXNAMELENGTH],tindex[MAXNAMELENGTH];
    char temp[MAXLINELENGTH];
    FILE *fp;
    fp = fopen(fname.c_str(),"r+");
    fseek(fp,0,SEEK_SET);
    fscanf(fp,"%d",&num);
    curnum = num;
    fpos_t tmp;
    //printf("%d\n",num);
    for(int i=0; i<curnum; i++)
    {
        fpos_t flag;
        fgetpos(fp,&flag);
        char tempc;
        fscanf(fp,"%c",&tempc);
        if(tempc==' ')
        {
            i--;
            fgets(temp,MAXLINELENGTH,fp);
            continue;
        }
        fsetpos(fp,&flag);
        fgets(temp,MAXLINELENGTH,fp);
        memset(temp,' ',strlen(temp)-1);
        fsetpos(fp,&flag);
        fscanf(fp,"%s",tindex);
        //printf("%s\n",tindex);
        if(!strcmp(tindex,indexName.c_str()))
        {
            fscanf(fp,"%s",ttable);
            fscanf(fp,"%s",tattr);
            fscanf(fp,"%d",&type);
            fgetpos(fp,&tmp);
            //this->revokeIndexOnAttribute(ttable,tattr,indexName);
            fsetpos(fp,&flag);
            fputs(temp,fp);
            //fprintf(fp,"\n");
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
    string fname = tableName;
    if(remove(fname.c_str()))
    {
        return 0;
    }
    return 1;
}

int CatalogManager::findIndex(string indexName)
{
    int num, flag=UNKNOWN_FILE;
    string fname = "indexes";
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
    string fname = tableName;
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
    string fname = "indexes";
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
    memset(tindex,0,MAXNAMELENGTH);
    string fname = tableName;
    FILE *fp;
    fp = fopen(fname.c_str(),"r");
    fseek(fp,0,SEEK_SET);
    for(int i=0; i<3; i++)
        fgets(temp,sizeof(temp),fp);
    fscanf(fp,"%d",&attrnum);
    for(int i=0 ; i<attrnum; i++)
    {
        fscanf(fp,"%s",tattr);
        fscanf(fp,"%d",&ttype);
        fscanf(fp,"%d",&tuniq);
        //        fgets(tindex,MAXNAMELENGTH*sizeof(char),fp);
        //        for(int j=1; j<MAXNAMELENGTH;j++)
        //        {
        //            if(tindex[j]==' ')
        //            {
        //                tindex[j]='\0';
        //                break;
        //            }
        //        }
        string attrName = tattr;
        string indexName = this->getIndexName( tableName, attrName);
        //cout<<"IndexName in function:"<<indexName<<endl;
        //        int num;
        //        string res="";
        //        char ttindex[MAXNAMELENGTH],tttable[MAXNAMELENGTH],ttattr[MAXNAMELENGTH];
        //        FILE *fp1 = fopen("indexes","r");
        //        fscanf(fp1,"%d",&num);
        //        for(int i=0; i<num; i++)
        //        {
        //            fscanf(fp1,"%s",ttindex);
        //            fscanf(fp1,"%s",tttable);
        //            fscanf(fp1,"%s",ttattr);
        //            //if(!strcmp(tttable,tableName.c_str())&& !strcmp(ttattr,attrName.c_str()))
        //            if(1)
        //            {
        //                res = tindex;
        //            }
        //        }
        Attribute curattr(tattr,ttype,tuniq,indexName);
        //cout<<curattr.index<<"12345\n";
        attributeVector->push_back(curattr);
    }
    fclose(fp);
    return attrnum;
}

int CatalogManager::getBlockNum(string tableName)
{
    int rn,len;
    rn = getRecordNum(tableName);
    len = calculateLength(tableName) + 1;
    double num = rn*1.0/(blockSize*1.0/len);
    return (int)ceil(num);
}

string CatalogManager::getIndexName(string tableName, string attrName)
{
    int num;
    string res="";
    char tindex[MAXNAMELENGTH],ttable[MAXNAMELENGTH],tattr[MAXNAMELENGTH];
    char rub[MAXLINELENGTH];
    FILE *fp = fopen("indexes","r");
    fscanf(fp,"%d",&num);
    for(int i=0; i<num; i++)
    {
        fscanf(fp,"%s",tindex);
        fscanf(fp,"%s",ttable);
        fscanf(fp,"%s",tattr);
        fgets(rub,sizeof(rub),fp);
        if(!strcmp(ttable,tableName.c_str())&&!strcmp(tattr,attrName.c_str()))
        {
            res = tindex;
        }
    }
    fclose(fp);
    return res;
}


int CatalogManager::getIndexNameList(string tableName, vector<string> *indexNameVector)
{
    int num;
    string filename = "indexes";
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
    string filename = "indexes";
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
    fclose(fp);
    return -2;
}

int CatalogManager::getRecordNum(string tableName)
{
    int recordNum;
    char temp[MAXLINELENGTH];
    string fname = tableName;
    FILE *fp;
    fp = fopen(fname.c_str(),"r");
    fseek(fp,0,SEEK_SET);
    fgets(temp,sizeof(temp),fp);
    fgets(temp,sizeof(temp),fp);
    fscanf(fp,"%d",&recordNum);
    fclose(fp);
    return recordNum;
}

bool CatalogManager::getRecordString(string tableName, vector<string> *recordContent, char *recordResult)
{
    bool res = true;
    vector<Attribute> attributeVector;
    getAttributes(tableName, &attributeVector);
    char *contentBegin = recordResult;
    for(int i = 0; i < attributeVector.size(); i++)
    {
        Attribute attribute = attributeVector[i];
        string content = (*recordContent)[i];
        int type = attribute.type;
        int typeSize = calculateLength2(type);
        stringstream ss;
        ss << content;
        //cout<<ss<<"#####"<<endl;
        if (type == 0)
        {
            //if the content is a int
            if(content.length()>=13) return false;
            for(char c:content)
            {
                if(!(c>='0'&&c<='9')) return false;
            }
            int intTmp;
            ss >> intTmp;
            //cout<<intTmp<<"#####"<<endl;
            memcpy(contentBegin, &intTmp, typeSize);
        }
        else if (type == -1)
        {
            //if the content is a float
            if(content==".") return false;
            for(char c:content)
            {
                if(!((c>='0'&&c<='9')||c=='.')) return false;
            }
            float floatTmp;
            ss >> floatTmp;
            //cout<<floatTmp<<"#####"<<endl;
            memcpy(contentBegin, &floatTmp, typeSize);
            float asss;
            memcpy(&asss, contentBegin, sizeof(float));
            //cout << asss << endl;
        }
        else
        {
            //if the content is a string
            //cout<<"length:"<<content.length()<<endl;
            if(content.length()>type) return false;
            memcpy(contentBegin, content.c_str(), typeSize);
            char temp[MAXLINELENGTH];
            memcpy(temp,contentBegin,typeSize);
            memset(temp+content.length(),0,MAXLINELENGTH-content.length()-1);
            string t = temp;
            //cout<<t<<endl;
            memcpy(contentBegin,t.c_str(),typeSize);
        }
        contentBegin += typeSize;
    }
    
    //cout<<"@@@"<<contentBegin<<"@@@"<<endl;
    return res;
}

int CatalogManager::insertRecord(string tableName, int recordNum)
{
    int curRecordNum,tlen;
    char temp[MAXLINELENGTH];
    char tint[MAXINTLENGTH];
    string fname = tableName;
    FILE *fp;
    fp = fopen(fname.c_str(),"r+");
    fseek(fp,0,SEEK_SET);
    fgets(temp,sizeof(temp),fp);
    fgets(temp,sizeof(temp),fp);
    fscanf(fp,"%s",tint);
    //printf("%s\n",tint);
    tlen = strlen(tint);
    curRecordNum=atoi(tint);
    curRecordNum+=recordNum;
    string tmp = to_string(curRecordNum);
    memcpy(tint, tmp.c_str(), sizeof(tmp.c_str()));
    //printf("I am here2.\n");
    memset(tint+strlen(tint),' ',MAXINTLENGTH-strlen(tint)-1);
    //printf("I am here1.\n");
    tint[MAXINTLENGTH-1]='\0';
    fseek(fp,-tlen,SEEK_CUR);
    //fseek(fp, (-MAXINTLENGTH)*(long)(sizeof(char)), SEEK_CUR);
    //fwrite(&curRecordNum,sizeof(int),1,fp);
    //fprintf(fp,"%d\n",curRecordNum);
    //cout<<"### "<<tint<<" ###"<<endl;
    fputs(tint,fp);
    fprintf(fp,"\n");
    fclose(fp);
    return 1;
}

//int CatalogManager::revokeIndexOnAttribute(string tableName, string AttributeName, string indexName)
//{
//    int attrnum,tmp,i;
//    char temp[MAXLINELENGTH];
//    char tattr[MAXNAMELENGTH],tindex[MAXNAMELENGTH];
//    string fname = tableName;
//    FILE *fp;
//    fp = fopen(fname.c_str(),"r+");
//    fseek(fp,0,SEEK_SET);
//    for(int i=0; i<3; i++)
//        fgets(temp,sizeof(temp),fp);
//    fscanf(fp,"%d",&attrnum);
//    for(i=0 ;i<attrnum; i++)
//    {
//        fscanf(fp,"%s",tattr);
//        if(!strcmp(tattr,AttributeName.c_str()))
//        {
//            fscanf(fp,"%d",&tmp);
//            fscanf(fp,"%d",&tmp);
//            fscanf(fp,"%s",tindex);
//            //printf("### %s ###\n\n",tindex);
//            if(!strcmp(tindex,indexName.c_str()))
//            {
//                //printf("###I am here.###\n");
//                memset(temp,' ',MAXNAMELENGTH-1);
//                temp[MAXNAMELENGTH-1]='\0';
//                fseek(fp,0,SEEK_CUR);
//                fseek(fp,(long)(-1)*indexName.length()*sizeof(char),SEEK_CUR);
//                //fprintf(fp,"aaabbbccc\n");
//                fputs(temp,fp);
//                fprintf(fp,"\n");
//                break;
//            }
//            else
//            {
//                fclose(fp);
//                return 0;
//            }
//        }
//        else
//        {
//            fgets(temp,sizeof(temp),fp);//Deal with the extra information in the same line.
//        }
//    }
//    fclose(fp);
//    return (i<attrnum);
//}

//int CatalogManager::setIndexOnAttribute(string tableName, string AttributeName, string indexName)
//{
//    int attrnum,tmp,i;
//    char temp[MAXLINELENGTH];
//    char tattr[MAXNAMELENGTH];
//    char tindex[MAXNAMELENGTH];
//    strcpy(tindex,indexName.c_str());
//    memset(tindex+indexName.length(),' ',MAXNAMELENGTH-indexName.length()-1);
//    tindex[MAXNAMELENGTH-1]='\0';
//    string fname = tableName;
//    FILE *fp;
//    fp = fopen(fname.c_str(),"r+");
//    fseek(fp,0,SEEK_SET);
//    for(int i=0; i<3; i++)
//        fgets(temp,sizeof(temp),fp);
//    fscanf(fp,"%d",&attrnum);
//    for(i=0 ; i<attrnum; i++)
//    {
//        fscanf(fp,"%s",tattr);
//        if(!strcmp(tattr,AttributeName.c_str()))
//        {
//            //printf("\nYESYESYES\n");
//            fscanf(fp,"%d",&tmp);
//            fscanf(fp,"%d",&tmp);
//            fprintf(fp," ");
//            //printf("%s\n",indexName.c_str());
//            fseek(fp,0,SEEK_CUR);
//            fputs(tindex,fp);
//            //printf("%d##%s\n",sizeof(tindex),tindex);
//            //fprintf(fp,"\n");
//            break;
//        }
//        else
//        {
//            //printf("\nNoNoNo\n");
//            fgets(temp,sizeof(temp),fp);//Deal with the extra information in the same line.
//        }
//    }
//    fclose(fp);
//    return (i<attrnum);
//}
