#ifndef _LOGIC_H_
#define _LOGIC_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Functions.h"

using namespace std;

#define DEC_SEPARATOR '.'

class Constants
{
public:
	static Constants inst;		// instance

	static bool constantExists(string name)
	{
		if(inst.constants.find(name) == inst.constants.end())
			return false;
		else
			return true;
	}

	// constantExists should be called before to check 
	// if a constant of given name is on the list
	static double constantValue(string name)
	{
		return inst.constants[name];
	}

private:
	std::map <string, double> constants;

	// Constant names can only be formed with letters and digits
	Constants()					// TODO: add more constants			
	{
		constants["pi"] = 3.14159265359;
		constants["three"] = 3.00;
		constants["luckynumber"] = 7;
		constants["p1"] = 1.1111111;
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
		for(int i = 0; i < inst.operators.size();i++)
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

	static bool isPrioritized(string first, string second)
	{
		if(!isOperator(first) || !isOperator(second))
		{
			cout << first << " lub " << second << " to nie operator!" << endl;
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

string getNumber(string pattern, int& index)
{
	string number;

	while(isDigit(pattern[index]) && index < pattern.length())
	{
		number += pattern[index];
		index++;
	}
	if(pattern.length() != 0 && pattern[index] == DEC_SEPARATOR)
	{
		number += pattern[index];
		index++;
		if(!isDigit(pattern[index]))
			cout << "Brak liczby po separatorze dziesiÍtnym!";
		while(isDigit(pattern[index]) && index < pattern.length())
		{
			number += pattern[index];
			index++;
		}
	}

	return number;
}  

#endif // !_LOGIC_H_
