#include <iostream>
#include <string>
#include <vector>
using namespace std;

static string wyr = "( ( 5 - 2 ) ^ ( 3 + 1 ) / ( 2 + 1 ) + 12 ) * 21 = ";
static string output = "";
string number = "";
vector<char> stack;

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

bool isCorrect(string equation)
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

int main ()
{
	while(true)
	{
		std::getline(std::cin, wyr);
		if(wyr == "")
			break;
		output = "";
		if(!isCorrect(wyr))
			continue;
		for(int i=0; i<wyr.length(); ++i)
		{
			while(wyr[i] == ' ')
				i++;
			number = "";

			while(isNumber(wyr[i]) && i < wyr.length())
			{
				number += wyr[i];
				i++;
			}
			if(number.length() != 0 && wyr[i] == ',')
			{
				number += wyr[i];
				i++;
				if(!isNumber(wyr[i]))
					cout << "Przecinek a po nim brak liczby!";
				while(isNumber(wyr[i]) && i < wyr.length())
				{
					number += wyr[i];
					i++;
				}
			}

			appendOutput(number);

			while(wyr[i] == ' ')
				i++;

			if(i < wyr.length())
				switch(wyr[i])
				{
				case '=':
					while( !stack.empty() )
						appendOutput(stack.back()), stack.pop_back();
					output += wyr[i];
					if(i != wyr.length()-1)
						cout << "Znak = nie na koncu wyrazenia";
					break;
				case '(':
					stack.push_back(wyr[i]);
					break;
				case ')':
					while(!stack.empty() && stack.back() != '(')
						appendOutput(stack.back()), stack.pop_back();
					stack.pop_back();
					break;
				default:
					if( ! operators.isOperator(wyr[i]) )
						cout << "Error, nieznany znak: " << wyr[i];

					if(wyr[i] == '-' && i != 0)		//sprawdzamy wystepowanie (-a)
					{
						int tmp = i-1;
						while(wyr[tmp] == ' ')
							tmp--;
						if(wyr[tmp] == '(')			//obslugujemy je przez zapisanie 0 - a
						{
							appendOutput('0');
						}
					}

					while( !stack.empty() && stack.back() != '(' && !operators.isPrioritized(wyr[i], stack.back()) )
						appendOutput(stack.back()), stack.pop_back();
				
					stack.push_back(wyr[i]);
					break;
				}

		}
		while( !stack.empty() )
			appendOutput(stack.back()), stack.pop_back();

		cout << "Przed: " << wyr << endl;
		cout << "Po: " << output << endl;
	}
	return 0;
}