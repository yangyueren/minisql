#include "API.h"
using namespace std;
#define UNKNOWN_FILE 8
#define TABLE_FILE 9
#define INDEX_FILE 10


/*
	创建表，如有主键则同时建立索引，默认为primary_key_on_'table_name'
*/
void API::create_table(string table_name, vector<Attribute> &attribute_vector, int primary_key_location)
{
	// 该表已经存在
	if(catalog_manager.findTable(table_name) == TABLE_FILE){
		cout << "There exsits a table named " << table_name << endl;
		return;
	}
	// record 和 catalog处理
	if(record_manager.tableCreate(table_name)){
		catalog_manager.addTable(table_name, attribute_vector, primary_key_location);
		cout << "Create table " << table_name << " successfully" << endl;
	}
	else{
		cout << "Create table " << table_name << " fail" << endl;
	}

	if(primary_key_location < attribute_vector.size()){
		// 在主键上建索引
		string index_name = "primary_key_on_" + table_name;
		create_index(index_name, table_name, attribute_vector[primary_key_location].name);
	}

}
/*
	创建索引，只能建立unique index
*/
void API::create_index(string index_name, string table_name, string attribute_name)
{
	// 检查索引是否存在
	if(catalog_manager.findIndex(index_name) == INDEX_FILE){
		cout << "There exsits index named " << index_name << endl;
		return;
	}
	// 检查表是否存在
	if(check_table(table_name) == false){
		return;
	}
	// 建立索引
	vector<Attribute> attribute_vector;
	catalog_manager.getAttributes(table_name, &attribute_vector);
	// 预检查是否符合索引条件，是否满足unique
	int i; // 第几个属性是主键，后面传给record
	for(i = 0; i < attribute_vector.size(); i++){
		if(attribute_vector[i].name == attribute_name){
			if(attribute_vector[i].unique == false){
				cout << "The attribute is not unique" << endl;
				return;
			}
			break;
		}
	}
	if(i == attribute_vector.size()){
		cout << "This table has no attribute named " << attribute_name << endl;
		return;
	}

	// record, catalog存储
    
	if(record_manager.indexCreate(index_name)){ // 建立文件
		int type = attribute_vector[i].type;
		// int 4 float 4 char 256
		catalog_manager.addIndex(index_name, table_name, attribute_name, type);
		
		// index存储
		string index_file_name = index_file_name_get(index_name);
		if(type == -1){ // float
			index_manager.CreateIndex(index_file_name, 4, 1);
		}
		else if(type == 0){ // int
			index_manager.CreateIndex(index_file_name, 4, 0);
		}
		else{ // char
			index_manager.CreateIndex(index_file_name, type, 2, type);
		}
        
		// 在原有记录上建索引
		int recordSize = catalog_manager.calculateLength(table_name);
        int bn = catalog_manager.getBlockNum(table_name);
		record_manager.insertNewIndex(table_name, index_name, recordSize, bn, attribute_vector, i);
		cout << "Create index " << index_name << " successfully" << endl;
	}
	else{
		cout << "Create index " << index_name << " fail" << endl;
	}
}


void API::drop_table(string table_name)
{
	if(check_table(table_name) == false){ // table 不存在
		return;
	}
	// 首先要删除index，再删除表
	vector<string> index_name_vector;
	// 获得索引list
	catalog_manager.getIndexNameList(table_name, &index_name_vector);
	for(int i = 0; i < index_name_vector.size(); i++){
		// drop every index 
		// cout << index_name_vector[i] << endl;
		drop_index(index_name_vector[i]);
	}

	// record删除表
	if(record_manager.tableDrop(table_name)){
		catalog_manager.dropTable(table_name);
		cout << "Drop table " << table_name << " successfully" << endl;
	}
	else{
		cout << "Drop table " << table_name << " fail" << endl;
	}

}

void API::drop_index(string index_name)
{
	if(check_index(index_name) == false){ // index不存在
		return;
	}
	// record 删除索引信息
	if(record_manager.indexDrop(index_name)){
		int index_type = catalog_manager.getIndexType(index_name);
		if(index_type == -2){
			cout << "Error" << endl;
			return;
		}

		// 删除索引信息
		catalog_manager.dropIndex(index_name);
		// index 删除索引树
		string index_file_name = index_file_name_get(index_name);
		index_manager.DeleteIndex(index_file_name); // 删除树
        cout << "Drop index " << index_name << " successfully" << endl;
	}
}

void API::record_show(string table_name, vector<Condition> &condition_vector)
{
	if(check_table(table_name) == true){
		int num = 0;
		int offset = -1;
		vector<Attribute> attribute_vector;
        catalog_manager.getAttributes(table_name, &attribute_vector);
		// 输出attribute
		attribute_print(attribute_vector);
		// 区别有条件和无条件 size = 0 无条件， = 1 有条件
		if(condition_vector.size() != 0){
			for(int i = 0; i < condition_vector.size(); i++){
				int j;
				for(j = 0; j < attribute_vector.size(); j++){
					if(attribute_vector[j].name == condition_vector[i].Attribute_Name){
						if(condition_vector[i].operate == OPERATOR_EQUAL && attribute_vector[j].index != ""){
							offset = index_manager.SearchInIndex(index_file_name_get(attribute_vector[j].index), condition_vector[i].value, attribute_vector[j].type);
						}
						break;
					}
				}
				if(j == attribute_vector.size()){ 
					cout << "There dosn't exists the attribute" << endl;
					return;
				}
			}
		}
		int recordSize = catalog_manager.calculateLength(table_name);
		// 没有索引
		if(offset == -1){
            int bn = catalog_manager.getBlockNum(table_name);
			num = record_manager.showRecord(table_name, recordSize, bn, attribute_vector, condition_vector);
		}
		// 有索引
		else{
            int bn = catalog_manager.getBlockNum(table_name);
			num = record_manager.showClearRecord(table_name, recordSize, bn, attribute_vector, condition_vector, offset);
		}
		// 输出一共多少条记录
		cout << num << " rows in set" << endl;
	}
	else{
		return;
	}
}
void API::record_insert(string table_name, vector<string> &value_vector)
{
	// 检查表是否存在
	if(check_table(table_name) == false){
		return;
	}
    
	char record[2000];
	memset(record, 0 , 2000);
	// 从catalog获取record
	bool right = catalog_manager.getRecordString(table_name, &value_vector, record);
    if(right == false){
        cout << "Illegeal input" << endl;
        return;
    }
	// record增加记录，返回“存在第几个块”
	int recordSize = catalog_manager.calculateLength(table_name);

	
	// 检查索引
	vector<Attribute> attribute_vector;
	vector<Condition> condition_vector;
	catalog_manager.getAttributes(table_name, &attribute_vector);
	int i;
	for(i = 0; i < attribute_vector.size(); i++){
		string index_name = attribute_vector[i].index;
        // 检查索引中是否有这个值
		if(index_name != ""){ // 有索引
			int keytype = getType(attribute_vector[i].type);
			int blockOffset = index_manager.SearchInIndex(index_file_name_get(index_name), value_vector[i], keytype);
			if(blockOffset != -1){ // 索引中存在了这个值
				cout << "Error: duplicate entry "<< value_vector[i] << " for key " << attribute_vector[i].name << endl;
				return;
			}
			else{ // 索引中没有这个值， 通过索引进行插入
                // 在此之前判断是否满足unique条件
                
				for(int j = 0; j < attribute_vector.size(); j++){
                    if(attribute_vector[j].unique == true){
                        Condition condi(attribute_vector[j].name, value_vector[j], OPERATOR_EQUAL);
                        vector<Condition> new_condition_vector;
                        new_condition_vector.push_back(condi);

                        int bn = catalog_manager.getBlockNum(table_name);
                        int record_conflict = record_manager.searchRecord(table_name, recordSize, bn, attribute_vector, new_condition_vector);
                        if(record_conflict > 0){ // 该unique冲突了
                            cout << "Error: duplicate entry "<< value_vector[j] << " for key " << attribute_vector[j].name << endl;
                            return;
                        }
                    }
				}
				// record 存储记录
				int offset = record_manager.insertRecord(table_name, record, recordSize);
				if(offset >= 0){
					// index 增加索引
					index_manager.InsertIntoIndex(index_file_name_get(index_name), value_vector[i], keytype, offset);
					// catalog 增加一条数量
					catalog_manager.insertRecord(table_name, 1);
					cout << "Insert successfully" << endl;
					return; // 有索引的插入，插入完直接结束
				}
				else{
					cout << "Insert fail" << endl;
					return;
				}
			}
		}
		else if(attribute_vector[i].unique == true){ // 没有索引 但unique，将该条件
			Condition condi(attribute_vector[i].name, value_vector[i], OPERATOR_EQUAL);
			condition_vector.insert(condition_vector.end(), condi);
		}
	}
	// 如果存在unique，判断unique中是否有冲突
    
	if(condition_vector.size() > 0){
		for(int i = 0; i < condition_vector.size(); i++){
            int bn = catalog_manager.getBlockNum(table_name);
            vector<Condition> new_condition_vector;
            new_condition_vector.push_back(condition_vector[i]);
			int record_conflict = record_manager.searchRecord(table_name, recordSize, bn, attribute_vector, new_condition_vector);
			if(record_conflict > 0){ // 该unique冲突了
				cout << "Error: duplicate entry "<< condition_vector[i].value << " for key " << condition_vector[i].Attribute_Name << endl;
				return;
			}
		}
	}
	// 没有索引的插入
	int offset = record_manager.insertRecord(table_name, record, recordSize);
	if(offset >= 0){
		catalog_manager.insertRecord(table_name, 1);
		cout << "Insert successfully" << endl;
	}
	else{
		cout << "Insert fail" << endl;
	}
}

void API::update(string table_name, string attribute_name, string new_value, vector<Condition> &condition_vector)
{
	if(check_table(table_name) == false){
		return;
	}
	int update_position = -1;
    int index_position;
	string index_name = "";
	vector<Attribute> attribute_vector;
	catalog_manager.getAttributes(table_name, &attribute_vector);
	// 检查要更改的attribute是否存在
	int i;
	for(i = 0; i < attribute_vector.size(); i++){
		if(attribute_vector[i].name == attribute_name){
			update_position = i;
			break;
		}
	}
	if(i == attribute_vector.size()){ // attribute不存在
		cout << "Error: There is no attribute named " << attribute_name << endl;
		return;
	}
	// 检查condition中是否有索引-> 没有 / 有
	int offset = -1;
	if(condition_vector.size() != 0){
		for(int i = 0; i < condition_vector.size(); i++){
			int j;
			for(j = 0; j < attribute_vector.size(); j++){
				if(attribute_vector[j].name == condition_vector[i].Attribute_Name){
					if(condition_vector[i].operate == OPERATOR_EQUAL && attribute_vector[j].index != ""){
						index_name = attribute_vector[j].index;
                        index_position = j;
						offset = index_manager.SearchInIndex(index_file_name_get(index_name), condition_vector[i].value, attribute_vector[j].type);
					}
					break;
				}
			}
			if(j == attribute_vector.size()){ 
				cout << "There dosn't exist the attribute" << endl;
				return;
			}
		}
	}
	Condition condi(attribute_name, new_value, OPERATOR_EQUAL); // 更新后的值传进去
	condition_vector.insert(condition_vector.begin(), condi); // condition_vector[0]
	int recordSize = catalog_manager.calculateLength(table_name);
	// 没有索引
	if(offset == -1){
        int bn = catalog_manager.getBlockNum(table_name);
		int num = record_manager.updateRecord(table_name, recordSize, bn, attribute_vector, condition_vector);
		if(num == 0){
			cout << "No record matches the conditions" << endl;
			return;
		}
		cout << num << " record updated" << endl;
	}
	else{// 有索引
		int num = record_manager.updateClearRecord(table_name, recordSize, attribute_vector, condition_vector, offset);
		if(num == 0){
			cout << "No record matches the conditions" << endl;
			return;
		}
		else{
			cout << num << " record updated" << endl;
            if(index_position != -1)
                update_index(index_name, table_name, attribute_vector[index_position].name);
		}
	}

}
void API::quit()
{
    index_manager.writeToDisk();
    return;
}
void API::attribute_print(vector<Attribute> &attribute_vector)
{
	for(int i = 0; i < attribute_vector.size(); i++){
		if(i != attribute_vector.size() - 1 ){
			cout << attribute_vector[i].name << ", ";
		}
		else{ // the last one
			cout << attribute_vector[i].name << endl;
		}
	}
}
string API::index_file_name_get(string index_name)
{
	return "IndexFile_" + index_name;
}
bool API::check_table(string table_name)
{
	if(catalog_manager.findTable(table_name) != TABLE_FILE){
		cout << "There is no table named " << table_name << endl;
		return false;
	}
	else{
		return true;
	}
}
bool API::check_index(string index_name)
{
	if(catalog_manager.findIndex(index_name) != INDEX_FILE){
		cout << "There is no index named " << index_name << endl;
		return false;
	}
	else{
		return true;
	}
}
int API::getType(int type)
{
	if(type == 0){ // int 
		return 0;
	}
	else if(type == -1){ // float
		return 1;
	}
	else{ //char
		return 2;
	}
}
void API::record_delete(string table_name, vector<Condition> &condition_vector)
{
    // 检查表是否存在
    if(check_table(table_name) == false){
        return;
    }
    int blockOffset = -1;
    int position = -1;
    vector<Attribute> attribute_vector;
    catalog_manager.getAttributes(table_name, &attribute_vector);
    string index_name = "";
    if(condition_vector.size() != 0){ // 有条件
        for(int i = 0; i < condition_vector.size(); i++){
            for(int j = 0; j < attribute_vector.size(); j++){
                if(attribute_vector[j].name == condition_vector[i].Attribute_Name && attribute_vector[j].index != ""){
                    index_name = attribute_vector[j].index;
                    blockOffset = index_manager.SearchInIndex(index_file_name_get(index_name), condition_vector[i].value, attribute_vector[j].type);
                    if(blockOffset != -1){
                        position = j;
                        break;
                    }
                }
            }
        }
    }
    int num = 0;
    int recordSize = catalog_manager.calculateLength(table_name);
    blockOffset = -1;
    
    if(blockOffset == -1){ // 没有索引
        int bn = catalog_manager.getBlockNum(table_name);
        num = record_manager.deleteRecord(table_name, recordSize, bn, attribute_vector, condition_vector);
    }
    else{ // 有索引
        int bn = catalog_manager.getBlockNum(table_name);
        num = record_manager.deleteClearRecord(table_name, recordSize, bn, attribute_vector, condition_vector, blockOffset);
    }
    //catalog_manager.deletevalue(table_name, num);
    if(num != 0){
        update_index(index_name, table_name, attribute_vector[position].name);
        cout << "delete " << num << " successfully" << endl;
    }
    else{
        cout << "No such record" << endl;
    }
}
void API::update_index(string index_name, string table_name, string attribute_name)
{
    if(check_index(index_name) == false){ // index不存在
        return;
    }
    // record 删除索引信息
    if(record_manager.indexDrop(index_name)){
        int index_type = catalog_manager.getIndexType(index_name);
        if(index_type == -2){
            //cout << "Error" << endl;
            return;
        }
        
        // 删除索引信息
        catalog_manager.dropIndex(index_name);
        // index 删除索引树
        string index_file_name = index_file_name_get(index_name);
        index_manager.DeleteIndex(index_file_name); // 删除树
        // cout << "Drop index " << index_name << " successfully" << endl;
    }
    // 检查索引是否存在
    if(catalog_manager.findIndex(index_name) == INDEX_FILE){
        // cout << "There exsits index named " << index_name << endl;
        return;
    }
    // 检查表是否存在
    if(check_table(table_name) == false){
        return;
    }
    // 建立索引
    vector<Attribute> attribute_vector;
    catalog_manager.getAttributes(table_name, &attribute_vector);
    // 预检查是否符合索引条件，是否满足unique
    int i; // 第几个属性是主键，后面传给record
    for(i = 0; i < attribute_vector.size(); i++){
        if(attribute_vector[i].name == attribute_name){
            if(attribute_vector[i].unique == false){
                //cout << "The attribute is not unique" << endl;
                return;
            }
            break;
        }
    }
    if(i == attribute_vector.size()){
        //cout << "This table has no attribute named " << attribute_name << endl;
        return;
    }
    
    // record, catalog存储
    
    if(record_manager.indexCreate(index_name)){ // 建立文件
        int type = attribute_vector[i].type;
        // int 4 float 4 char 256
        catalog_manager.addIndex(index_name, table_name, attribute_name, type);
        
        // index存储
        string index_file_name = index_file_name_get(index_name);
        if(type == -1){ // float
            index_manager.CreateIndex(index_file_name, 4, 1);
        }
        else if(type == 0){ // int
            index_manager.CreateIndex(index_file_name, 4, 0);
        }
        else{ // char
            index_manager.CreateIndex(index_file_name, type, 2, type);
        }
        
        // 在原有记录上建索引
        int recordSize = catalog_manager.calculateLength(table_name);
        int bn = catalog_manager.getBlockNum(table_name);
        record_manager.insertNewIndex(table_name, index_name, recordSize, bn, attribute_vector, i);
        //cout << "Create index " << index_name << " successfully" << endl;
    }
    else{
        //cout << "Create index " << index_name << " fail" << endl;
    }
}
