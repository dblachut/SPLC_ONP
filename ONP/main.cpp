#include "ONP_translator.h"

int main ()
{
	string line;
	while(true)
	{
		std::getline(std::cin, line);
		if(line == "")
			break;

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