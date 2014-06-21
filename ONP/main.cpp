#include "ONP_translator.h"
#include "Parser.h"

int main ()
{
	string line;
	while(true)
	{
		std::getline(std::cin, line);
		if(line == "")
			break;

		//check if the formula is correct
		if(!isFormulaCorrect(line)){
			cout << "Incorrect formula\n";
			continue;
		}else{
			cout << "Formula correct\n";
		}

		//get user parameters values here
		line = getUserValues(line);

		//parse formula ( make an equation out of it )
		line = parseFormula(line);
		
		//translate the equation to Reverse Polish Notation
		translateToONP(line);

		//calculate value TODO:
	}

	return 0;
}