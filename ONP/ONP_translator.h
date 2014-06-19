#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

#define DEC_SEPARATOR ','
static string output = "";

class Operators
{
public:
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

	bool isOperator(char c)
	{
		return std::find(operators.begin(), operators.end(), c) != operators.end();
	}

	bool isPrioritized(char first, char second)
	{
		if(!isOperator(first) || !isOperator(second))
		{
			cout << first << " lub " << second << " to nie operator!" << endl;
			return false;
		}
		int idx1 = std::distance(operators.begin(), std::find(operators.begin(), operators.end(), first));
		int idx2 = std::distance(operators.begin(), std::find(operators.begin(), operators.end(), second));

		return priority[idx1] > priority[idx2];
	}

private:
	vector<char> operators;
	vector<int>  priority;
} operators;

bool isNumber(char c)
{
	return c >= '0' && c <= '9';
}

void appendOutput(string text)
{
	if(text.length())
		output += text + ' ';
}

void appendOutput(char c)
{
	output += c;
	output += ' ';
}

bool isEquationCorrect(string equation)
{
	int operatorCount = 0;
	int openingBrackets = 0;
	int closingBrackets = 0;
	for(int i=0; i<equation.length(); ++i)
	{
		while(equation[i] == ' ')
			i++;
		if(operators.isOperator(equation[i]))
		{
			operatorCount++;
			if(i+1<equation.length() && equation[i+1] == ')')
			{
				cout << "Error operatora przed nawiasem!" << endl;
				return false;
			}
			if(operatorCount>1)
			{
				cout << "Error dwoch operatorow obok siebie!" << endl;
				return false;
			}
		}
		else
		{
			if(equation[i] == '(')
				openingBrackets++;
			else if(equation[i] == ')')
				closingBrackets++;
			operatorCount = 0;
		}
	}
	if(openingBrackets != closingBrackets)
	{
		cout << "Error nie zgadza sie liczba nawiasow!" << endl;
		return false;
	}
	return true;
}

void translateToONP(string equation)
{
	string number = "";
	vector<char> stack;

	for(int i=0; i<equation.length(); ++i)
		{
			while(equation[i] == ' ')
				i++;
			number = "";

			while(isNumber(equation[i]) && i < equation.length())
			{
				number += equation[i];
				i++;
			}
			if(number.length() != 0 && equation[i] == DEC_SEPARATOR)
			{
				number += equation[i];
				i++;
				if(!isNumber(equation[i]))
					cout << "Brak liczby po separatorze dziesiêtnym!";
				while(isNumber(equation[i]) && i < equation.length())
				{
					number += equation[i];
					i++;
				}
			}

			appendOutput(number);

			while(equation[i] == ' ')
				i++;

			if(i < equation.length())
				switch(equation[i])
				{
				case '=':
					while( !stack.empty() )
						appendOutput(stack.back()), stack.pop_back();
					output += equation[i];
					if(i != equation.length()-1)
						cout << "Znak = nie na koncu equationazenia";
					break;
				case '(':
					stack.push_back(equation[i]);
					break;
				case ')':
					while(!stack.empty() && stack.back() != '(')
						appendOutput(stack.back()), stack.pop_back();
					stack.pop_back();
					break;
				default:
					if( ! operators.isOperator(equation[i]) )
						cout << "Error, nieznany znak: " << equation[i];

					if(equation[i] == '-' && i != 0)		//sprawdzamy wystepowanie (-a)
					{
						int tmp = i-1;
						while(equation[tmp] == ' ')
							tmp--;
						if(equation[tmp] == '(')			//obslugujemy je przez zapisanie 0 - a
						{
							appendOutput('0');
						}
					}

					while( !stack.empty() && stack.back() != '(' && !operators.isPrioritized(equation[i], stack.back()) )
						appendOutput(stack.back()), stack.pop_back();
				
					stack.push_back(equation[i]);
					break;
				}

		}
		while( !stack.empty() )
			appendOutput(stack.back()), stack.pop_back();

		cout << "Przed: " << equation << endl;
		cout << "Po: " << output << endl;
}