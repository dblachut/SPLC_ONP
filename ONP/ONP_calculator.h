#ifndef _ONPCALC_H_
#define _ONPCALC_H_
#include "Functions.h"

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <cstdlib>

double calculateONP(string line)
{
	std::istringstream iss(line);
	std::vector<double> stack;
	std::cout << "Input\t\tOperation\t\tStack after" << std::endl;
	std::string token;
	while (iss >> token)
	{
		std::cout << token << "\t\t";
		double tokenNum;
		if (std::istringstream(token) >> tokenNum)
		{
			std::cout << "Push\t\t\t";
			stack.push_back(tokenNum);
		}
		else
		{
			std::cout << "Operate\t\t\t";			
			if(Operators::isOperator(token))
			{
				int argc = Operators::getOperator(token).getArgc();
				vector<double> tmp;

				if(argc == -1)	//dynamic amount of arguments
					argc = stack.back(), stack.pop_back();

				for (int i = 0; i < argc; i++)
				{
					tmp.push_back(stack.back());
					stack.pop_back();
				}
				reverse(tmp.begin(),tmp.end());
				stack.push_back( Operators::getOperator(token).getFunction()(tmp) );
			} // todo = in next line
			else if (token == "=");
			// do nothing
			else
			{ //just in case
				std::cerr << "Error" << std::endl;
				std::exit(1);
			}
		}
		std::copy(stack.begin(), stack.end(), std::ostream_iterator<double>(std::cout, " "));
		std::cout << std::endl;
	}
	return stack.back();
}

#endif // !_ONPCALC_H_
