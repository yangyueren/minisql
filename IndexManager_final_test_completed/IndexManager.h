#pragma once
#include "BPlusTree.h"
#include <string>

using namespace std;

struct Index
{
	string IndexFileName;
	int Type;
	BPlusTree<int>  B_Plus_Tree_int;
	BPlusTree<float>  B_Plus_Tree_float;
	BPlusTree<string>  B_Plus_Tree_string;
};


class IndexManager
{
public:
	IndexManager();
	~IndexManager();
	bool GetIndex(string IndexName, int Key_Type);//传入一个索引名（实际为文件名）和键的类型，若已存在，将索引头加载到内存，返回true，否则返回false
	//bool CreateIndex(string IndexName, int KeySize, int Key_Type, int Degree);//传入一个索引名（实际为文件名）、建索引的值得大小、建索引的值的类型、树的degree，创建一个空索引，成功返回true，否则返回false
	//bool CreateIndex(string IndexName, int KeySize, int Key_Type);//传入一个索引名（实际为文件名）、建索引的值得大小、建索引的值的类型，创建一个空索引，成功返回true，否则返回false
	//bool InsertIntoIndex(string IndexName, string KeyValue, int Key_Type, OffsetType Offset);//传入插入的索引名，插入索引的值（表中该字段的值），插入的值的类型（一定不能有错），该值在record文件中的偏移量，返回插入是否成功。
	//OffsetType SearchInIndex(string IndexName, string KeyValue, int Key_Type);//传入搜索的索引名，需要搜索的值（表中该字段的值），搜索的值的类型（一定不能有错），返回该值在record文件中的偏移量
	//bool DeleteFromIndex(string IndexName, string KeyValue, int Key_Type);//传入删除的索引名，需要删除的值（表中该字段的值），删除的值的类型（一定不能有错），返回删除是否成功。
	//bool DeleteIndex(string IndexName);//在传入删除的索引文件名，在内存中删除索引头
	bool CreateIndex(string IndexName, int KeySize, int Key_Type);//int float   传入一个索引名（实际为文件名）、建索引的值得大小、建索引的值的类型，创建一个空索引，成功返回true，否则返回false
	bool CreateIndex(string IndexName, int KeySize, int Key_Type, int Degree);// string   传入一个索引名（实际为文件名）、建索引的值得大小、建索引的值的类型、树的degree是char的长度，创建一个空索引，成功返回true，否则返回false
	OffsetType SearchInIndex(string IndexName, string KeyValue, int Key_Type);           //-1代表没有
	bool InsertIntoIndex(string IndexName, string KeyValue, int Key_Type, OffsetType Offset);// string IndexName, string KeyValue, int Key_Type, OffsetType Offset
	bool DeleteInIndex(string IndexName, string KeyValue, int Key_Type);
	bool DeleteIndex(string IndexName);//在传入删除的索引文件名，在内存中删除索引头

	void levelListIndex(string IndexName);
private:
	vector<struct Index> IndexSet;
	int IndexNum;
};




/*
bool CreateIndex(string IndexName, int KeySize, int Key_Type);//int float   传入一个索引名（实际为文件名）、建索引的值得大小、建索引的值的类型，创建一个空索引，成功返回true，否则返回false
bool CreateIndex(string IndexName, int KeySize, int Key_Type, int Degree);// string   传入一个索引名（实际为文件名）、建索引的值得大小、建索引的值的类型、树的degree是char的长度，创建一个空索引，成功返回true，否则返回false
OffsetType SearchInIndex(string indexName,  key, keytype)           //-1代表没有
bool InsertIntoIndex(string indexName, key, keytype, OffsetType Offset)// 
bool DeleteInIndex(string indexName, key, keytype);
bool DeleteIndex(string IndexName);//在传入删除的索引文件名，在内存中删除索引头

*/