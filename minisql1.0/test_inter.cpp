#include "Interpreter.hpp"

int main()
{
	string s="";
	u_int t = 0;
	Interpreter in;
	getline(cin,s,';');
	cout << s << endl;
	in.interpreter(s);
}
