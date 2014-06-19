#include "ONP_translator.h"

int main ()
{
	string line;
	while(true)
	{
		std::getline(std::cin, line);
		if(line == "")
			break;

		if(!isEquationCorrect(line))
			continue;
		
		translateToONP(line);
	}
	return 0;
}