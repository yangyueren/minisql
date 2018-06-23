#ifndef _Minisql_IndexInfos_h
#define _Minisql_IndexInfos_h

#include <string>
using namespace std;

class IndexInfo
{
public:
	IndexInfo(string i,string t,string a,int ty)
    {indexName = i;tableName = t;attributeName = a;type = ty;}
    string indexName;
    string tableName;
    string attributeName;
    int type;
};

#endif
