#include "ONP_translator.h"
#include "Parser.h"
#include "ONP_calculator.h"

int main ()
{
	string line;
	vector<string> formulas;
	while(true)
	{
		std::getline(std::cin, line);
		if(line == "")
			break;
		replaceAll(line, " ", "");	//get rid of all spaces
		//check if the formula is correct
		if(!isFormulaCorrect(line))
		{
			cout << "Incorrect formula\n";
			continue;
		}
		else
		{
			cout << "Formula correct\n";
		}

		//parse formula ( make an equation out of it )
		formulas = parseFormula(line);

		if(!isEquationCorrect(line))
		{
			cout << "Incorrect equation\n";
			continue;
		}
		else
		{
			cout << "Equation correct\n";
		}
		
		for each (string fromula in formulas)
		{
			//translate the equation to Reverse Polish Notation
			line = translateToONP(fromula);
			if(line == "")
				continue;
			//calculate value
			cout << "Result = " << calculateONP(line) << endl;
		}
	}

	return 0;
}