#ifndef MINISQL_ATTRIBUTE_HPP
#define MINISQL_ATTRIBUTE_HPP

#include <string>
#include <iostream>
using namespace std;
#define TYPE_FLOAT -1
#define TYPE_INT 0
class Attribute
{
public:
	string name; // the name of the attribute
	int type; // the type of the attribute, -1 represents float, 0 represents int, 1~255 represents char
	bool unique; // true represents unique, false-default represents not unique
	string index; // ""-default represents no index
	Attribute(string na, int t, bool un = false, bool pri = false, string in = "" ): name(na), type(t), unique(un), index(in){}

	string index_name_get(){
		return index;
	}
	
	// for attribute test
	void print(){
		cout << name << endl;
		cout << type << endl;
		cout << unique << endl;
		cout << index << endl;
	}
};

#endif