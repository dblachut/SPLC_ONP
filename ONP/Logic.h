#ifndef _LOGIC_H_
#define _LOGIC_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include "Functions.h"

using namespace std;
typedef unsigned int uint;
#define DEC_SEPARATOR '.'
#define ARG_SEPARATOR ','

void toUpper(string&);

class Constants
{
public:
	static Constants inst;		// instance

	static bool constantExists(string name)
	{
		toUpper(name);
		return inst.constants.find(name) != inst.constants.end();
	}

	// constantExists should be called before to check 
	// if a constant of given name is on the list
	static double constantValue(string name)
	{
		toUpper(name);
		return inst.constants[name];
	}

	// return value as string
	static string constantStringValue(string name)
	{
		toUpper(name);
		return std::to_string(inst.constants[name]);
	}

private:
	std::map <string, double> constants;

	// Constant names can only be formed with letters and digits
	Constants()					// TODO: add more constants			
	{
		constants["PI"] = 3.14159265359;
		constants["THREE"] = 3.00;
		constants["LUCKYNUMBER"] = 7;
		constants["P1"] = 1.1111111;
	}
};

Constants Constants::inst = Constants();

class Operator
{
public:
	Operator(string name, int priority, int argc, double (*function)(vector<double>))
	{
		this->name = name;
		this->priority = priority;
		this->argc = argc;
		this->function = function;
	}

	Operator(char name, int priority, int argc, double (*function)(vector<double>))
	{
		Operator(string(1,name),priority,argc,function);
	}

	string getName()
	{
		return name;
	}

	int getPriority()
	{
		return priority;
	}

	int getArgc()
	{
		return argc;
	}

	double(*getFunction())(vector<double>)
	{
		return function;
	}

	Operator() // warning, creates an invalid object.
	{
	}
private:
	string name;
	int priority;
	int argc;
	double (*function)(vector<double>);
};

class Operators // IMPORTANT: function names are considered to be operators as well in the algorithm
{
public:

	static Operators inst;

	static Operator getOperator(string name)
	{
		Operator a;
		for(int i = 0; i < inst.operators.size(); i++)
		{
			if(inst.operators[i].getName() == name)
			{
				a = inst.operators[i];
				break;
			}
		}
		return a;
	}

	static bool isOperator(char c)
	{
		return isOperator(string(1,c));
	}

	static bool isOperator(string name)
	{
		for(int i = 0; i < inst.operators.size();i++)
		{
			if(inst.operators[i].getName() == name)
				return true;
		}
		return false;
	}

	static bool isFunction(string name)
	{
		for(int i = 0; i < inst.operators.size();i++)
		{
			if(inst.operators[i].getName() == name)
				return inst.operators[i].getName().length() != 1; //operators are one chars
		}
		return false;
	}

	static bool isPrioritized(string first, string second)
	{
		if(!isOperator(first) || !isOperator(second))
		{
			cout << first << " or " << second << " is not an Operator!" << endl;
			return false;
		}
		int p1,p2;
		for(int i = 0; i < inst.operators.size();i++)
		{
			if(inst.operators[i].getName() == first)
				p1 = inst.operators[i].getPriority();
			if(inst.operators[i].getName() == second)
				p2 = inst.operators[i].getPriority();
		}

		return p1 > p2;
	}

	static bool isPrioritized(string first, char second){return isPrioritized(first,string(1,second));}
	static bool isPrioritized(char first, string second){return isPrioritized(string(1,first),second);}
	static bool isPrioritized(char first, char second){return isPrioritized(string(1,first),string(1,second));}

private:

	Operators()
	{
		//string oper[7] = {"^", "*", "/", ":", "%", "+", "-"};
		//int  prio[7] = { 3,   2,   2,   2,   2,   1,   1 };

		operators.push_back(Operator("^", 3, 2, powFunction));		
		operators.push_back(Operator("*", 2, 2, multiplyFunction));
		operators.push_back(Operator("/", 2, 2, divideFunction));
		operators.push_back(Operator(":", 2, 2, divideIntFunction));
		operators.push_back(Operator("%", 2, 2, moduloFunction));
		operators.push_back(Operator("+", 1, 2, addFunction));
		operators.push_back(Operator("-", 1, 2, subtractFunction));

		operators.push_back(Operator("sin", 4, 1, sinFunction));
		operators.push_back(Operator("cos", 4, 1, cosFunction));
		operators.push_back(Operator("sqrt", 4, 1, sqrtFunction));
		operators.push_back(Operator("pow", 4, 2, powFunction));
		operators.push_back(Operator("rand", 4, 0, randFunction));	
		operators.push_back(Operator("fact", 4, 1, factorialFunction));
		operators.push_back(Operator("avg", 4, -1, averageFunction));	
	}

	vector<Operator> operators;
};

Operators Operators::inst = Operators();

bool isDigit(char c)
{
	return c >= '0' && c <= '9';
}

bool isLetter(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

void toUpper(string &text)
{
	std::transform(text.begin(), text.end(),text.begin(), ::toupper);
}

// get number from pattern, starding from index
// this function implements decimal separator and increments index to value after number
string getNumber(string pattern, int& index)
{
	string number;

	while(index < pattern.length() && isDigit(pattern[index]))
		number += pattern[index++];

	if(pattern.length() != 0 && pattern[index] == DEC_SEPARATOR)
	{
		number += pattern[index++];

		if(!isDigit(pattern[index]))
			throw("No digit after decimal separator!");

		while(index < pattern.length() && isDigit(pattern[index]))
			number += pattern[index++];
	}

	return number;
}

string getNumberWithMinus(string pattern, int& index)
{
	string number;

	if(pattern[index] == '-')
		number += pattern[index++];

	while(index < pattern.length() && isDigit(pattern[index]))
		number += pattern[index++];

	if(pattern.length() != 0 && pattern[index] == DEC_SEPARATOR)
	{
		number += pattern[index++];

		if(!isDigit(pattern[index]))
			throw("No digit after decimal separator!");

		while(index < pattern.length() && isDigit(pattern[index]))
			number += pattern[index++];
	}

	return number;
}

// get name (name contains letters and digits) from pattern, starding from index
// this function increments index to value after name
string getName(string pattern, int& index)
{
	string name;

	while( (index < pattern.length()) && (isLetter(pattern[index]) || isDigit(pattern[index])) )
		name += pattern[index++];

	return name;
}

string getNameWithSpaces(string pattern, int& index)
{
	string name;

	while( (index < pattern.length()) && (isLetter(pattern[index]) || isDigit(pattern[index]) || pattern[index] == ' ') )
		name += pattern[index++];

	return name;
}

void replaceAll(std::string& str, const std::string& from, const std::string& to)
{
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos)
	{
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

#endif // !_LOGIC_H_
