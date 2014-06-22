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
				cout << "Error operator before ) bracket!" << endl;
				return false;
			}
			if(operatorCount>1)
			{
				cout << "Error two operators cannot be next to eachother!" << endl;
				return false;
			}
		}
		else
		{
			if(equation[i] == '(')
				openingBrackets++;
			else if(equation[i] == ')')
			{
				closingBrackets++;
				if(i+1<equation.length() && equation[i+1] != ')' && equation[i+1] != ',' && !Operators::isOperator(equation[i+1]))
				{
					cout << "Error no operator after closing bracket!" << endl;
					return false;
				}
			}
			
			operatorCount = 0;
		}
	}
	if(openingBrackets != closingBrackets)
	{
		cout << "Error wrong number of () brackets!" << endl;
		return false;
	}
	return true;
}

string translateToONP(string equation)
{
	string number = "";
	output = "";
	vector<string> stack;

	if(equation[0] == '-')
	{
		equation = "0" + equation;
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
						cout << "= sign not on the end of equation\n";
					break;
				case '(':
					stack.push_back(string(1,equation[i]));
					break;
				case ')':
					while(!stack.empty() && stack.back() != "(")
						appendOutput(stack.back()), stack.pop_back();
					stack.pop_back();
					break;
				case ARG_SEPARATOR: //TODO: coma or semicolon?
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
					}
					else
					{ // either a function or an error
						if(!isLetter(equation[i])) // error
						{
							cout << "Error, unknown char: " << equation[i] << endl;
							return "";
						}
						// Get function name
						string fName = "";
						while(equation[i] != '(')					
							fName+=equation[i++];
						i--;

						// recognize the function and put it where it belongs
						if(!Operators::isOperator(fName))
						{
							cout << "Error, function " << fName << " does not exist\n";
							return "";
						}

						stack.push_back(fName);
					}
			}

	}
	while( !stack.empty() )
		appendOutput(stack.back()), stack.pop_back();

	cout << "Przed: " << equation << endl;
	cout << "Po: " << output << endl;
	return output;
}  
#endif // !_TRANSLATOR_H_
