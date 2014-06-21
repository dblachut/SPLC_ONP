#ifndef _TRANSLATOR_H_
#define _TRANSLATOR_H_
#include "Logic.h"
#include "Functions.h"

static string output = "";

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
		if(Operators::isOperator(equation[i]))
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
	output = "";
	vector<string> stack;

	if(equation[0] == '-')
	{		
		string tmp = "0";
		tmp.append(equation);
		equation = tmp;
	}

	for(int i=0; i<equation.length(); ++i)
	{
		while(equation[i] == ' ')
			i++;

		number = getNumber(equation, i);
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
					cout << "Znak = nie na koncu równania";
				break;
			case '(':
				stack.push_back(string(1,equation[i]));
				break;
			case ')':
				while(!stack.empty() && stack.back() != "(")
					appendOutput(stack.back()), stack.pop_back();
				stack.pop_back();
				break;
			case ',': //TODO: coma or semicolon?
				// do nothing
				break;
			default:
				if( Operators::isOperator(equation[i]) )
				{
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

					while( !stack.empty() && stack.back() != "(" && !Operators::isPrioritized(equation[i], stack.back()) )
						appendOutput(stack.back()), stack.pop_back();

					stack.push_back(string(1,equation[i]));
					break;
				}else{ // either a function or an error
					if(!isLetter(equation[i])) // error
					{
						cout << "Error, nieznany znak: " << equation[i];
						return;
					}
					// Get function name
					string fName = "";
					while(equation[i] != '(')					
						fName+=equation[i++];
					i--;

					// recognize the function and put it where it belongs
					if(!Operators::inst.isOperator(fName))//Functions::inst.functionExists(fName))
					{
						cout << "Error, function " << fName << " does not exist\n";
						return;
					}

					stack.push_back(fName);
				}
		}

	}
	while( !stack.empty() )
		appendOutput(stack.back()), stack.pop_back();

	cout << "Przed: " << equation << endl;
	cout << "Po: " << output << endl;
}  
#endif // !_TRANSLATOR_H_
