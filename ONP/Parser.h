#include <string>
#include <iostream>
#include <map>

using namespace std;

std::map<string, double> constraints;

bool isFormulaCorrect(string formula)
{
	int openingBrackets = 0;
	int closingBrackets = 0;

	for(int i=0; i<formula.length(); ++i)
	{
		while(formula[i] == ' ')
			i++;
		
		if(formula[i] == '{')
		{
			openingBrackets++;
			if(i+1 >= formula.length() || formula[i+1] != '$')
			{
				cout << "Error brak znaku $ w argumencie!" << endl;
				return false;
			}
		}
		else if(formula[i] == '}')
			closingBrackets++;
	}

	if(openingBrackets != closingBrackets)
	{
		cout << "Error bledna liczba nawiasow argumentow!" << endl;
		return false;
	}

	cout << "Potrzebujemy: " << openingBrackets << "argumentow uzytkownika." << endl;

	return true;
}


void parseFormula(string formula)
{

}
