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

		//TODO: substitute constant names and functions with values

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