#ifndef API_H
#define API_H

#include "Attribute.hpp"
#include "Condition.hpp"
#include "RecordManager.h"
#include "CatalogManager.h"
#include "IndexManager.h"
#include <string>
#include <cstring>
#include <vector>
#include <stdio.h>

class API{

public:
	RecordManager record_manager;
	CatalogManager catalog_manager;
	//IndexManager index_manager;
	// attribute_vector存储表的所有attribute，location存储主键在vector中是第几个
	void create_table(string table_name, vector<Attribute> & attribute_vector, int primary_key_location);
	// 索引名 | 表名 | 列名
	void create_index(string index_name, string table_name, string attribute_name);
	void drop_table(string table_name);
	void drop_index(string index_name);
	// 表名 | condition_vector存储筛选条件，member有属性Attribute_Name、操作符operate、比较的值value
	void record_show(string table_name, vector<Condition> &condition_vector);
	// 表名 | value_vector存储属性值
	void record_insert(string table_name, vector<string> &value_vector);
	// 表名 | condition_vector存储筛选条件，member有属性Attribute_Name、操作符operate、比较的值value
	void record_delete(string table_name, vector<Condition> &condition_vector);
	void update(string table_name, string attribute_name, string new_value, vector<Condition> &condition_vector);
    void quit();
private:
	void attribute_print(vector<Attribute> &attribute_vector);
	bool check_table(string table_name);
	bool check_index(string index_name);
	void attribute_name_print(string table_name);
	string index_file_name_get(string index_name);
	int getType(int type);
};

#endif
