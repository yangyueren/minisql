#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP
#include <vector>
#include <string>
#include "Condition.hpp"
#include "Attribute.hpp"
#include "API.h"
typedef unsigned int u_int;
using namespace std;
class Interpreter
{
public:
	API *api;
	string file_name;
	int interpreter(string s);
	Interpreter(){}
	~Interpreter(){}

	// function for test
	void create_table_print(string s, vector<Attribute>  &attribute_vector, int primary_key_location); // done
	void create_index_print(string table_name, string index_name, string attribute_name); // done
	void record_show_print(string table_name, vector<Condition> &condition_vector); // done
	void drop_table_print(string table_name);
	void drop_index_print(string index_name);
	void record_insert_print(string table_name, vector<string> &value_vector); // done
	void record_delete_print(string table_name, vector<Condition> &condition_vector);
	void update_print(string table_name, string attribute_name, string new_value, vector<Condition> &condition_vector);
private:
	string split(string s, u_int &cur);
};

#endif

/*
	返回 ( , ) 或者 string
*/
string Interpreter::split(string s, u_int &cur)
{
	string word = "";
	u_int cur1, cur2;
	// ' = 39 space = 32 return\r = 13 newline\n = 10  tab\t = 9
	while(s[cur] != 0 && (s[cur] == 32 || s[cur] == 10 || s[cur] == 9)){
		cur++;
	}
	cur1 = cur;
	// handle
	if(s[cur] == '(' || s[cur] == ',' || s[cur] == ')'){
		cur++;
		cur2 = cur;
		word = s.substr(cur1, cur2 - cur1); // get word from ( or , or )
	}
	else if(s[cur] == 39){ // '
		cur++;
		while(s[cur] != 0 && s[cur] != 39){
			cur++;
		}
		if(s[cur] == 39){ // next '
			cur1++;
			cur2 = cur++; // cur2 = cur, cur++;
			word = s.substr(cur1, cur2 - cur1); // get word from '...'
		}
		else{
			cout << "Syntax Error: You may miss '" << endl;
			return "";
		}
	}
	else{
		while(s[cur] != 0 && s[cur] != 32 && s[cur] != 10 && s[cur] != '(' && s[cur] != ')' && s[cur] != ','){
			cur++;
		}
		cur2 = cur;
		if(cur1 != cur2)
			word = s.substr(cur1, cur2 - cur1);
	}
	return word;
}
/*
	  |	返回值 |    说明    |
		 -1 	  quit
		  0       error
		  1		 success
		  2		 execfile
	实现：
	create table: create_table(table_name, attribute_vector, primary_key_location)
	create index: create_index(index_name, table_name, attribute_name)
	drop table: drop_table(table_name)
	drop index: drop_index(index_name)
	select: record_show(table_name, condition_vector)
		or  record_show(table_name)
	insert: record_insert(table_name, value_vector)
	delete: record_delete(table_name, condition_vector)
		or	record_delete(table_name)

*/
int Interpreter::interpreter(string s)
{
	u_int cur = 0;
	string word = split(s, cur);
	if(word.compare("create") == 0){
		word = split(s, cur); // table or index or error
		if(word.compare("table") == 0){ // create table
			string primary_key = "";
			bool is_primary_key = false;
			int primary_key_location = -1;
			string table_name = "";
			word = split(s, cur);
			if(word.empty() != true){
				table_name = word;
			}
			else{
				cout << "Syntax Error for no table name" << endl;
				return 0;
			}
			word = split(s, cur);
			if(word.length() == 0 || word.compare("(") != 0 ){
				cout << "Syntax Error" << endl;
				return 0;
			}
			else{
				word = split(s, cur);
				vector<Attribute> attribute_vector;
				while(word.length() != 0 && word.compare("primary") != 0 && word.compare(")") != 0){
					string attribute_name = word; // record the attribute name
					int type;
					bool unique = false;
					// bool primary_key = false;
					word = split(s, cur);
					
					// deal with the date type
					if(word.compare("int") == 0){
						type = TYPE_INT;
					}
					else if(word.compare("float") == 0){
						type = TYPE_FLOAT;
					}
					else if(word.compare("char") == 0){
						word = split(s, cur);
						if(word.length() == 0 || word.compare("(") != 0){
							cout << "Syntax Error" << endl;
							return 0;
						}
						else{
							word = split(s, cur);
							istringstream convert(word);
							if(!(convert >> type)){
								cout << "Syntax Error: illegal length of type char" << endl;
								return 0;
							}
							if(type > 255 || type < 1){
								cout << " Syntax Error: illegal range of type char"<< endl;
								return 0;
							}
							word = split(s, cur); // ) or error
							if(word.compare(")") != 0){
								cout << "Syntax Error: You might miss )" << endl;
								return 0;
							}
						}
					}
					else{
						cout << "Syntax Error" << endl;
						return 0;
					}

					word = split(s, cur); // unique or , or )
					// deal with unique
					if(word.compare("unique") == 0){
						unique = true;
						word = split(s, cur);
					}
					Attribute col(attribute_name, type, unique);
					attribute_vector.push_back(col);
					if(word.compare(",") != 0){
						if(word.compare(")") == 0){ // create table done
							api->create_table(table_name, attribute_vector, primary_key_location);
							return 1;
						}
						else{
							cout << "Syntax Error: You may miss ," << endl;
							return 0;
						}
					}
					word = split(s, cur); // primary or error
					if(word.compare("primary") == 0){
						word = split(s, cur); // key or error
						if(word.compare("key") == 0){
							word = split(s, cur); // ( or error
							if(word.compare("(") != 0){ // error
								cout << "Syntax Error: You may miss (" << endl;
								return 0;
							}
							else{ // (
								word = split(s, cur);
								primary_key = word;
								if(is_primary_key == false){
									int i;
									for(i = 0; i < attribute_vector.size(); i++){
										if(attribute_vector[i].name == primary_key){
											attribute_vector[i].unique = true;
											is_primary_key = true;
											primary_key_location = i;
											break;
										}
									}
									if(i == attribute_vector.size()){ // error
										cout << "No such attribute exsit for primary key" << endl;
										return 0;
									}
								}
								else{ // error
									cout << "Repeating primary key definition" << endl;
									return 0;
								}
								word = split(s, cur); // ) or error
								if(word.compare(")") != 0){ 
									cout << "Syntax Error: You may miss )" << endl;
									return 0;
								}
								// for test print(table_name, attribute_vector, primary_key_location);
								api->create_table(table_name, attribute_vector, primary_key_location);
								return 1;
							}
						}
					}
				}
			}
		}
		else if(word.compare("index") == 0){ // create index
			string index_name = ""; // index name
			string table_name = "";
			string attribute_name = "";
			word = split(s, cur); // index name or error
			if(word.length()!=0){
				index_name = word;
				word = split(s, cur); // on or error
				if(word.compare("on") == 0){ // on
					word = split(s, cur); // on table
					if(word.length() != 0){
						table_name = word;
						word = split(s, cur); // ( or error
						if(word.compare("(") != 0){ // error
							cout << "Syntax Error: You may miss (" << endl;
							return 0;
						}
						else{
							word = split(s, cur); // col name or error
							if(word.length() != 0){ // col name
								attribute_name = word;
								word = split(s, cur); // ) or error
								if(word.compare(")") != 0){
									cout << "Syntax Error: You may miss )" << endl;
									return 0;
								}
								else{
									create_index_print(index_name, table_name, attribute_name);
									api->create_index(index_name, table_name, attribute_name);
									return 1;
								}
							}
							else{ // error
								cout << "Syntax Error: You may miss attribute name" << endl;
								return 0;
							}
						}

					}
					else{
						cout << "Syntax Error: You may miss table" << endl;
						return 0;							
					}
				}
				else{ // error
					cout << "Syntax Error: You may miss 'on'" << endl;
					return 0;
				}
			}
			else{ // error
				cout << "Syntax Error: You may miss index name" << endl;
				return 0;
			}

		}
		else{ // error
			cout << "Syntax Error: Wrong input, you can create table or index" << endl;
			return 0;
		}
	}
	
	else if(word.compare("drop") == 0){
		word = split(s, cur); // table or index or error
		if(word.compare("table") == 0){ // table
			string table_name = "";
			word = split(s, cur); // table name or error
			if(word.length() != 0){ // table name
				table_name = word;
				drop_table_print(table_name);
				api->drop_table(table_name);
				return 1;
			} 
			else{
				cout << "Syntax Error: Wrong table name" << endl;
				return 0;
			}

		}
		else if(word.compare("index") == 0){ // index
			string index_name = "";
			word = split(s, cur); // index name or error
			if(word.length() != 0){
				index_name = word;
				drop_index_print(index_name);
				api->drop_index(index_name);
				return 1;
			}
			else{
				cout << "Syntax Error: Wrong index name" << endl;
				return 0;
			}
		}
		else{
			cout << "Syntax Error: Wrong input, you can drop table or index" << endl;
			return 0;
		}
	}
	
	else if(word.compare("select") == 0){
		string table_name = "";
		word = split(s, cur); // * or error
		if(word.compare("*") == 0 ){ // *
			word = split(s, cur); // from or error
			if(word.compare("from") == 0){
				word = split(s, cur); // table name or error
				if(word.length() != 0){ // where
					table_name = word;
					word = split(s, cur); // where
					if(word.compare("where") == 0){
						vector<Condition> condition_vector;
						string attribute_name = "";
						string value = "";
						int operate;
						word = split(s, cur);
						if(word.length() != 0)
							while(1){
								attribute_name = word; // 还没有做判断
								word = split(s, cur); // operate
								if(word.compare("=") == 0)
									operate = OPERATOR_EQUAL;
								else if(word.compare("<>") == 0)
									operate = OPERATOR_NOT_EQUAL;
								else if(word.compare("<") == 0)
									operate = OPERATOR_LESS;
								else if(word.compare(">") == 0)
									operate = OPERATOR_GREATER;
								else if(word.compare("<=") == 0)
									operate = OPERATOR_LESS_EQUAL;
								else if(word.compare(">=") == 0)
									operate = OPERATOR_GREATER_EQUAL;
								else{ // error
									cout << "Syntax Error: wrong operater or You may miss conditions" << endl;
									return 0;
								}
								word = split(s, cur); // value
								if(word.length()!= 0)
									value = word;
								else{ // error
									cout << "Syntax Error: Incomplete conditions" << endl;
									return 0;
								}
								Condition row(attribute_name, value, operate);
								condition_vector.push_back(row);
								word = split(s, cur);
								if(word.length() == 0)
									break;
								if(word == "and")
									word = split(s, cur);
							}
						else{ // error
							cout << "Syntax Error: You may miss conditions" << endl;
							return 0;
						}
						record_show_print(table_name, condition_vector);
						api->record_show(table_name, condition_vector);
						return 1;
					}
					else if(word.length() == 0){ // select all
						vector<Condition> condition_vector;
						api->record_show(table_name, condition_vector);
						return 1;
					}
					else{
						cout << "Syntax Error: You may miss conditions" << endl;
						return 0;
					}
				}
				else{
					cout << "Syntax Error: You may miss table name" << endl;
					return 0;
				}
			}
			else{ // error
				cout << "Syntax Error: You may miss 'from'" << endl;
				return 0;
			}
		}
		else{ // error
			cout << "Syntax Error5" << endl;
			return 0;
		}
	}
	
	else if(word.compare("insert") == 0){
		string table_name = "";
		vector<string> value_vector;
		word = split(s, cur); // into or error
		if(word.length() != 0){
			word = split(s, cur); // table name
			if(word.length() != 0){
				table_name = word;
				word = split(s, cur); // values
				if(word == "values"){
					word = split(s, cur); // 
					if(word == "("){
						word = split(s, cur); // value or , or ) or error
						while(1){
							if(word == ")"){
								break;
							}
							else if(word == ","){
								word = split(s, cur);
								continue;
							}
							else if(word.length() != 0){
								value_vector.push_back(word);
								word = split(s, cur);
								continue;
							}
							else{
								cout << "Syntax Error" << endl;
								return 0;
							}
						}
						record_insert_print(table_name, value_vector);
						api->record_insert(table_name, value_vector);
						return 1;
					}
					else{
						cout << "Syntax Error: You may miss '('" << endl;
						return 0;
					}
				}
				else{
					cout << "Syntax Error: You may miss 'values'" << endl;
					return 0;
				}
			}
			else{
				cout << "Syntax Error" << endl;
				return 0;
			}
		}
	}
	
	else if(word.compare("delete") == 0){
		string table_name;
		word = split(s, cur);
		if(word == "from"){
			word = split(s, cur);
			if(word.length() != 0){
				table_name = word;
				word = split(s, cur); // "" or where or error
				if(word.length() == 0){
					vector<Condition> condition_vector;
					api->record_delete(table_name, condition_vector);
					return 1;
				}
				else if(word == "where"){
					vector<Condition> condition_vector;
					string attribute_name = "";
					string value = "";
					int operate;
					word = split(s, cur); // attribute name or error
					if(word.length() != 0){
						while(1){
							attribute_name = word;
							word = split(s, cur);
							if(word.compare("=") == 0)
								operate = OPERATOR_EQUAL;
							else if(word.compare("<>") == 0)
								operate = OPERATOR_NOT_EQUAL;
							else if(word.compare("<") == 0)
								operate = OPERATOR_LESS;
							else if(word.compare(">") == 0)
								operate = OPERATOR_GREATER;
							else if(word.compare("<=") == 0)
								operate = OPERATOR_LESS_EQUAL;
							else if(word.compare(">=") == 0)
								operate = OPERATOR_GREATER_EQUAL;
							else{ // error
								cout << "Syntax Error: wrong operater or You may miss conditions" << endl;
								return 0;
							}							
							word = split(s, cur); // value
							if(word.length()!= 0)
								value = word;
							else{ // error
								cout << "Syntax Error: wrong conditions" << endl;
								return 0;
							}
							Condition row(attribute_name, value, operate);
							condition_vector.push_back(row);
							word = split(s, cur);
							if(word.length() == 0)
								break;
							if(word == "and")
								word = split(s, cur);
						}
						record_delete_print(table_name, condition_vector);
						api->record_delete(table_name, condition_vector);
						return 1;
					}		
					else{
						cout << "Syntax Error: You may miss conditions" << endl;
						return 0;
					}
				}
				else{
					cout << "Syntax Error: You may miss 'where'" << endl;
					return 0;		
				}
			}
			else{
				cout << "Syntax Error: You may miss table" << endl;
				return 0;
			}
		}


		else{ // error
			cout << "Syntax Error: You may miss 'from'" << endl;
			return 0;
		}
	}
	// update table_name set attrubute = newvalue where attribute_name = value;
	else if(word == "update"){
		word = split(s, cur); // table name or error
		string table_name = "";
		string attribute_name = "";
		string new_value = "";
		if(word.length() != 0){ // table name
			table_name = word;
			word = split(s, cur); // set or error
			if(word == "set"){
				word = split(s, cur); // attribute name or error
				if(word.length() != 0){ // attribute name
					attribute_name = word;					
					word = split(s, cur); // =
					if(word == "="){
						word = split(s, cur); // new value or error
						if(word.length() != 0){
							new_value = word;
							word = split(s, cur); // where or error
							if(word == "where"){
								vector<Condition> condition_vector;
								string attribute_name2 = "";
								string value = "";
								int operate;
								word = split(s, cur); // attribute name
								if(word.length() != 0){
									while(1){
										attribute_name2 = word; // 还没有做判断
										word = split(s, cur); // operate
										if(word.compare("=") == 0)
											operate = OPERATOR_EQUAL;
										else{ // error
											cout << "Syntax Error: wrong operater or You may miss conditions" << endl;
											return 0;
										}
										word = split(s, cur); // value
										if(word.length()!= 0)
											value = word;
										else{ // error
											cout << "Syntax Error: Incomplete conditions" << endl;
											return 0;
										}
										Condition row(attribute_name2, value, operate);
										condition_vector.push_back(row);
										word = split(s, cur);
										if(word.length() == 0)
											break;
										if(word == "and")
											word = split(s, cur);
									}
									update_print(table_name, attribute_name, new_value, condition_vector);
									api->record_show(table_name, condition_vector);
								return 1;
								}
								else{ // error
									cout << "Syntax Error: You may miss conditions" << endl;
									return 0;
								}
							}
							else{ // error
								cout << "Syntax Error: You may miss 'where'" << endl;
								return 0;
							}

						}
						else{
							cout << "Syntax Error: You may miss new value to set" << endl;
							return 0;
						}
					}
					else{
						cout << "Syntax Error: You may miss '='" << endl;
						return 0;
					}
				}
				else{
					cout << "Syntax Error: You may miss attribute name" << endl;
					return 0;
				}
			}
			else{
				cout << "Syntax Error: You may miss 'set'" << endl;
				return 0;
			}
		}	
		else{
			cout << "Syntax Error: You may miss table name" << endl;
			return 0;
		}
	}
	else if(word == "quit"){
		return -1;
	}
	else if(word == "execfile"){
		
		string file_name = split(s, cur);
		cout << "try to open" << file_name << "..." << endl;
		return 2;
	}
	else{
		cout << "Syntax Error" << endl;
		return 0;
	}
	return 0;
}

// test 
void Interpreter::create_table_print(string s, vector<Attribute> &attribute_vector, int primary_key_location)
{
	int i;
	cout << s << endl;
	cout << attribute_vector.size() << endl;
	for(i = 0; i < attribute_vector.size(); i++){
		cout << attribute_vector[i].name << endl;
	}
	if(primary_key_location != -1){
		cout << primary_key_location << endl;
		cout << "primary_key: " << attribute_vector[primary_key_location].name << endl;
	
	}
}
void Interpreter::create_index_print(string table_name, string index_name, string attribute_name)
{
	cout << table_name << "," << index_name << "," << attribute_name << endl;
}
void Interpreter::drop_table_print(string table_name)
{
	cout << table_name << endl;
}
void Interpreter::drop_index_print(string index_name)
{
	cout << index_name << endl;
}
void Interpreter::record_show_print(string table_name, vector<Condition> &condition_vector)
{
	cout << table_name << endl;
	for(int i = 0; i < condition_vector.size(); i++){
		cout << condition_vector[i].Attribute_Name << "," << condition_vector[i].value << "," << condition_vector[i].operate << endl;
	}	
}
void Interpreter::record_insert_print(string table_name, vector<string> &value_vector)
{
	cout << table_name << endl;
	for(int i = 0; i < value_vector.size(); i++){
		cout << value_vector[i] << endl;
	}		
}
void Interpreter::record_delete_print(string table_name, vector<Condition> &condition_vector)
{
	cout << table_name << endl;
	for(int i = 0; i < condition_vector.size(); i++){
		cout << condition_vector[i].Attribute_Name << "," << condition_vector[i].value << "," << condition_vector[i].operate << endl;
	}
}
void Interpreter::update_print(string table_name, string attribute_name, string new_value, vector<Condition> &condition_vector)
{
	cout << "table_name:" << table_name << endl;
	cout << "attribute_name:" << attribute_name << endl;
	cout << "new_value:" << new_value << endl;
	for(int i = 0; i < condition_vector.size(); i++){
		cout << condition_vector[i].Attribute_Name << "," << condition_vector[i].value << "," << condition_vector[i].operate << endl;
	}	
}
