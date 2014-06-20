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

		if(!isFormulaCorrect(line)){
			cout << "Incorrect formula\n";
			continue;
		}else{
			cout << "Formula correct\n";
		}

		// TODO: get user parameters values here


		//substitute constant names with values
		line = insertConstantValues(line);
		
		// TODO: substitute functions with their calculated values NOTE: user arguments need to be filled out here already


		if(!isEquationCorrect(line)){
			cout << "Incorrect equation\n";
			continue;
		}else{
			cout << "Equation correct\n";
		}
		
		translateToONP(line);
	}
	return 0;
}