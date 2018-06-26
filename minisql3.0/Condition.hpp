#ifndef MINISQL_Condition_HPP
#define MINISQL_Condition_HPP
#include <string>
#include <sstream>
using namespace std;

#define OPERATOR_EQUAL 0 // =
#define OPERATOR_NOT_EQUAL 1 // <>
#define OPERATOR_LESS 2 // <
#define OPERATOR_GREATER 3 // >
#define OPERATOR_LESS_EQUAL 4 // <=
#define OPERATOR_GREATER_EQUAL 5 // >=
class Condition
{
public:
	string Attribute_Name;
	string value; // the value to be compared
	int operate; // the type to be compared
	
	Condition(string str, string val, int oper): Attribute_Name(str), value(val), operate(oper){}
	
	bool judge(int content)
	{
		stringstream str;
		str << value;
		int my_content;
		str >> my_content;
		switch(operate){
			case OPERATOR_EQUAL:
				return content == my_content;
				break;
			case OPERATOR_NOT_EQUAL:
				return content != my_content;
				break;
			case OPERATOR_LESS:
				return content < my_content;
				break;
			case OPERATOR_GREATER:
				return content > my_content;
				break;
			case OPERATOR_LESS_EQUAL:
				return content <= my_content;
				break;
			case OPERATOR_GREATER_EQUAL:
				return content >= my_content;
				break;
			default:
				return true;
				break;
		}
	}
	bool judge(float content)
	{
		stringstream ss;
		ss << value;
		float my_content;
		ss >> my_content;
		switch(operate){
			case OPERATOR_EQUAL:
				return content == my_content;
				break;
			case OPERATOR_NOT_EQUAL:
				return content != my_content;
				break;
			case OPERATOR_LESS:
				return content < my_content;
				break;
			case OPERATOR_GREATER:
				return content > my_content;
				break;
			case OPERATOR_LESS_EQUAL:
				return content <= my_content;
				break;
			case OPERATOR_GREATER_EQUAL:
				return content >= my_content;
				break;
			default:
				return true;
				break;
		}
	}
	bool judge(string content)
	{
		string my_content = value;
		switch(operate){
			case OPERATOR_EQUAL:
				return content == my_content;
				break;
			case OPERATOR_NOT_EQUAL:
				return content != my_content;
				break;
			case OPERATOR_LESS:
				return content < my_content;
				break;
			case OPERATOR_GREATER:
				return content > my_content;
				break;
			case OPERATOR_LESS_EQUAL:
				return content <= my_content;
				break;
			case OPERATOR_GREATER_EQUAL:
				return content >= my_content;
				break;
			default:
				return true;
				break;
		}
	}
};

#endif /* Condition.hpp */