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
	bool CreateIndex(string IndexName, int KeySize, int Key_Type);//int float   传入一个索引名（实际为文件名）、建索引的值得大小、建索引的值的类型，创建一个空索引，成功返回true，否则返回false
	bool CreateIndex(string IndexName, int KeySize, int Key_Type, int Degree);// string   传入一个索引名（实际为文件名）、建索引的值得大小、建索引的值的类型、树的degree是char的长度，创建一个空索引，成功返回true，否则返回false
	OffsetType SearchInIndex(string IndexName, string KeyValue, int Key_Type);           //-1代表没有
	bool InsertIntoIndex(string IndexName, string KeyValue, int Key_Type, OffsetType Offset);// string IndexName, string KeyValue, int Key_Type, OffsetType Offset
	bool DeleteInIndex(string IndexName, string KeyValue, int Key_Type);
	bool DeleteIndex(string IndexName);//在传入删除的索引文件名，在内存中删除索引头
	void writeToDisk();
	void levelListIndex(string IndexName, int Key_Type);
	bool SearchInRange(string IndexName, int Key_Type, string min_KeyValue, bool isLeftEqual,
		string max_KeyValue, bool isRightEqual, vector<int> &return_result);
private:
	vector<struct Index> IndexSet;
	int IndexNum;
};

extern IndexManager index_manager;



