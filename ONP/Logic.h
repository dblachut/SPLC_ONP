#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define DEC_SEPARATOR ','

class Operators
{
public:
	
	static Operators inst;

	static bool isOperator(char c)
	{
		return std::find(inst.operators.begin(), inst.operators.end(), c) != inst.operators.end();
	}

	static bool isPrioritized(char first, char second)
	{
		if(!isOperator(first) || !isOperator(second))
		{
			cout << first << " lub " << second << " to nie operator!" << endl;
			return false;
		}
		int idx1 = std::distance(inst.operators.begin(), std::find(inst.operators.begin(), inst.operators.end(), first));
		int idx2 = std::distance(inst.operators.begin(), std::find(inst.operators.begin(), inst.operators.end(), second));

		return inst.priority[idx1] > inst.priority[idx2];
	}

private:

	Operators()
	{
		char oper[7] = {'^', '*', '/', ':', '%', '+', '-'};
		int  prio[7] = { 3,   2,   2,   2,   2,   1,   1 };

		for(int i=0; i<7; i++)
		{
			operators.push_back(oper[i]);
			priority.push_back(prio[i]);
		}
	}

	vector<char> operators;
	vector<int>  priority;
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
			cout << "Brak liczby po separatorze dziesiêtnym!";
		while(isDigit(pattern[index]) && index < pattern.length())
		{
			number += pattern[index];
			index++;
		}
	}

	return number;
}