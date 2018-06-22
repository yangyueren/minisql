#include "API.h"
#include "RecordManager.h"
#include "CatalogManager.h"
#include "IndexManager.h"
using namespace std;
#define UNKNOWN_FILE 8
#define TABLE_FILE 9
#define INDEX_FILE 10
RecordManager *record_manager;
CatalogManager *catalog_manager;
IndexManager *index_manager;
//
///*
//	创建表，如有主键则同时建立索引，默认为primary_key_on_'table_name'
//*/
void API::create_table(string table_name, vector<Attribute> &attribute_vector, int primary_key_location)
{
	// 该表已经存在
	if(catalog_manager->findTable(table_name) == TABLE_FILE){
		cout << "There exsits a table named " << table_name << endl;
		return;
	}
	// record 和 catalog处理
	if(record_manager->tableCreate(table_name)){
		catalog_manager->addTable(table_name, attribute_vector, primary_key_location);
		cout << "Create table " << table_name << "successfully" << endl;
	}
	else{
		cout << "Create table " << table_name << "fail" << endl;
	}

	//if(primary_key_location < attribute_vector.size()){
	//	// 在主键上建索引
	//	string index_name = "primary_key_on_" + table_name;
	//	create_index(index_name, table_name, attribute_vector[primary_key_location].name);
	//}

}
///*
//	创建索引，只能建立unique index
//*/
//void API::create_index(string index_name, string table_name, string attribute_name)
//{
//	// 检查索引是否存在
//	if(catalog_manager->findIndex(index_name) == INDEX_FILE){
//		cout << "There exsits index named " << index_name << endl;
//		return;
//	}
//	// 检查表是否存在
//	if(check_table(table_name) == false){
//		return;
//	}
//	// 建立索引
//	vector<Attribute> attribute_vector;
//	catalog_manager->attributeGet(table_name, &attribute_vector);
//	// 预检查是否符合索引条件，是否满足unique
//	int i;
//	for(i = 0; i < attribute_vector.size(); i++){
//		if(attribute_vector[i].name == attribute_name){
//			if(attribute_vector[i].unique == false){
//				cout << "The attribute is not unique" << endl;
//				return;
//			}
//			break;
//		}
//	}
//	if(i == attribute_vector.size()){
//		cout << "This table has no attribute named " << attribute_name << endl;
//		return;
//	}
//
//	// record, catalog存储
//	if(record_manager->indexCreate(index_name)){
//		int type = attribute_vector[i].type;
//		// int 4 float 4 char 256
//		catalog_manager->addIndex(index_name, table_name, attribute_name, type);
//		
//		// index存储
//		string index_file_name = "Index_file" + index_name;
//		if(type == -1){ // float
//			index_manager->CreateIndex(index_file_name, 1);
//		}
//		else if(type == 0){ // int
//			index_manager->CreateIndex(index_file_name, 0);
//		}
//		else{ // char
//			index_manager->CreateIndex(index_file_name, 2, type);
//		}
//
//		// 在原有记录上建索引 record_manager->
//		cout << "Create index " << index_name << "successfully" << endl;
//	}
//	else{
//		cout << "Create index " << index_name << "fail" << endl;
//	}
//}
///*
//	
//*/
//void API::drop_table(string table_name)
//{
//	if(check_table(table_name) == false){ // table 不存在
//		return;
//	}
//	// 首先要删除index，再删除表
//	vector<string> index_name_vector;
//	// 获得索引list
//	catalog_manager->indexNameListGet(table_name, index_name_vector);
//	for(int i = 0; i < index_name_vector.size(); i++){
//		// drop every index 
//		// cout << index_name_vector[i] << endl;
//		drop_index(index_name);
//	}
//
//	// record删除表
//	if(record_manager->tableDrop(table_name)){
//		catalog_manager->dropTable(table_name);
//		cout << "Drop table " << table_name << "successfully" << endl;
//	}
//	else{
//		cout << "Drop table " << table_name << "fail" << endl;
//	}
//
//}
///*
//
//*/
//void API::drop_index(string index_name)
//{
//	if(check_index(index_name) == false){ // index不存在
//		return;
//	}
//	// record 删除索引信息
//	if(record_manager->indexDrop(index_name)){
//		int index_type = catalog_manager->getIndexType(index_name);
//		if(index_type == -2){
//			cout << "Error" << endl;
//			return;
//		}
//
//		// 删除索引信息
//		catalog_manager->dropIndex(index_name);
//		// index 删除索引树
//		string index_file_name = "Index_file" + index_name;
//		index_manager->DeleteIndex(index_file_name); // 删除树
//	}
//}
///*
//
//*/
//void API::record_show(string table_name, vector<Condition> &condition_vector)
//{
//	if(check_table(table_name) == true){
//		int num = 0;
//		int offset = -1;
//		vector<Attribute> attribute_vector;
//		catalog_manager->attributeGet(table_name, &attribute_vector);
//		// 输出attribute
//		attribute_print(attribute_vector);
//		// 区别有条件和无条件 size = 0 无条件， = 1 有条件
//		if(condition_vector.size() != 0){
//			for(int i = 0; i < condition_vector.size(); i++){
//				int j;
//				for(j = 0; j < attribute_vector.size(); j++){
//					if(attribute_vector[j].name == condition_vector[i].Attribute_Name){
//						if(condition_vector[i].operate == OPERATOR_EQUAL && attribute_vector[j].index != ""){
//							offset = index_manager->searchIndex(record_manager->indexFileNameGet(attribute_vector[j].index), condition_vector[i].value, attribute_vector[j].type);
//						}
//						break;
//					}
//				}
//				if(j == attribute_vector.size()){ 
//					cout << "There dosn't exists the attribute" << endl;
//					return;
//				}
//			}
//		}
//		else{ // 无条件，所有都输出
//			// 查看有无索引
//			offset = index_manager->searchIndex(index_file_name_get(index_name), condition_vector[i].value, attribute_vector[j].type);
//		}
//		int recordSize = catalog_manager->calculateLenth(table_name);
//		// 没有索引
//		if(offset == -1){
//			num = record_manager->showRecord(table_name, recordSize, bn, attribute_vector, condition_vector);
//		}
//		// 有索引
//		else{
//			num = record_manager->showClearRecord(table_name, recordSize, bn, attribute_vector, condition_vector, offset);
//		}
//		// 输出一共多少条记录
//		cout << num << " rows in set" << endl;
//	}
//	else{
//		return;
//	}
//}
//void API::record_insert(string table_name, vector<string> &value_vector)
//{
//	// 检查表是否存在
//	if(check_table(table_name) == false){
//		return;
//	}
//	
//	char record[2000];
//	memset(record, 0, 2000);
//	// 从catalog获取record
//	catalog_manager->recordStringGet(table_name, value_vector, record);
//	// record增加记录，返回“存在第几个块”
//	int recordSize = catalog_manager->calculateLenth(table_name);
//	int blockOffset = record_manager->insertRecord(table_name, record, recordSize);
//	// ... for index
//
//	// 判断有没有index
//	// 检查索引，
//	vector<Attribute> attribute_vector;
//	vector<Condition> condition_vector;
//	catalog_manager->attributeGet(attribute_vector);
//	int i;
//	for(i = 0; i < attribute_vector.size(); i++){
//		index_name = attribute_vector[i].index_name_get();
//		if(index_name != ""){ // 有索引
//
//		}
//		else if(attribute_vector[i].unique == true){ // 没有索引 但unique
//			
//		}
//	}
//	// 没有index unique, 遍历 查找是否冲突。
//
//}
//void API::record_delete(string table_name, vector<Condition> &condition_vector)
//{	
//	// 检查表是否存在
//	if(check_table(table_name) == false){
//		return;
//	}
//	
//	vector<Attribute> attribute_vector;
//	catalog_manager->attributeGet(table_name, &attribute_vector);
//
//	int num = 0;
//	num = record_manager->deleteRecord(table_name, recordSize, bn, attribute_vector, condition_vector);
//}
//
//void API::attribute_print(vector<Attribute> &attribute_vector)
//{
//	for(int i = 0; i < attribute_vector.size(); i++){
//		if(i != attribute_vector.size() - 1 ){
//			cout << attribute_vector[i].name << ", ";
//		}
//		else{ // the last one
//			cout << attribute_vector[i].name << endl;
//		}
//	}
//}
//string API::index_file_name_get(string index_name)
//{
//	return "Index_file" + index_name;
//}
bool API::check_table(string table_name)
{
	if(catalog_manager->findTable(table_name) != TABLE_FILE){
		cout << "There is no table named " << table_name << endl;
		return false;
	}
	else{
		return true;
	}
}
//bool API::check_index(string index_name)
//{
//	if(catalog_manager->findIndex(index_name) != INDEX_FILE){
//		cout << "There is no index named " << index_name << endl;
//		return false;
//	}
//	else{
//		return true;
//	}
//}