#ifndef _PARSER_H_
#define _PARSER_H_
#include <vector>
#include "Logic.h"
#include "Functions.h"

int gAlternatingArgPosition = 0; // I don't think its the best way to do it
int gAlternatingArgAmount = 0;

// checks if constraint name exist and increment index
bool checkConstantName(string pattern, int& index)
{
	if(index >= pattern.length() || !isLetter(pattern[index]))
	{
		cout << "Error no constant name after '#' sign!" << endl;
		return false;
	}

	string constName = getName(pattern, index);

	if( !Constants::constantExists(constName) )
	{
		cout << "Error const " << constName << " does not exists!" << endl;
		return false;
	}
	
	return true;
}

// checks if function has proper amount of agruments and sets it in dynamical one
// adidionally increment index
bool checkFunctionArgumentAmount(string &pattern, int& index)
{
	string fName = "";
	while(pattern[index] != '(')					
		fName+=pattern[index++];
	index++;
	if(!Operators::isOperator(fName))
	{
		cout << "Error, function " << fName << " does not exist\n";
		return false;
	}
	else
	{
		int argCount = Operators::getOperator(fName).getArgc();
		int argEntered = 0;
				
		for(index; index<pattern.length(); index++)
		{
			while(pattern[index] == ' ')
				index++;
			if(getNumber(pattern, index) != "")
				argEntered++;
			while(pattern[index] == ' ')
				index++;
			if(pattern[index] != ARG_SEPARATOR)
			{
				if(pattern[index] == ')')
					break;
				else if(isLetter(pattern[index]))
				{
					if(!checkFunctionArgumentAmount(pattern, index))
						return false;
					index--;
					argEntered++; //functon as an argment
				}
				else if(pattern[index] == '#')
				{
					index++;
					if(!checkConstantName(pattern, index))
						return false;
					index--;
					argEntered++; //const as an argument
				}
				else if(pattern[index] == '{')
				{
					while(pattern[index] != '}')
						index++;
					argEntered++; //user argument as argument
				}
				else
				{
					cout << "Error, unknown sign " << pattern[index] << " in function arguments\n";
					return false;
				}
			}
		}

		if(argCount == -1) //we need to add argument number due to dynamic amount of arguments
		{
			string toInsert = ARG_SEPARATOR + std::to_string(argEntered);
			pattern.insert(index,  toInsert);
			index += toInsert.length();
		}
		else if (argCount != argEntered)
		{
			cout << "Error, wrong amount of parameters for function " << fName << ". Got: " << argEntered << ", expected: " << argCount << endl;
			return false;
		}
		index++;
	}
}

bool checkAlternatingArgument(string pattern, int &index, vector<string> &enteredVals = vector<string>(), bool iWantValues = false)
{
	if(pattern[index] == ':')
	{
		gAlternatingArgPosition = index;
		vector<string> numbers;
		if(!iWantValues && pattern[index+1] != 'Z')
		{
			cout << "Error no Z sign after declaration (:) of alternating argument!" << endl;
			return false;
		}
		index += 2;
		if(!iWantValues && ++gAlternatingArgAmount > 1)
		{
			cout << "Error number of alternating arguments is limited to 1!" << endl;
			return false;
		}
		if(pattern[index] != '}')
		{
			if(pattern[index] == '(')
			{
				index++;
				if(pattern[index] != ')')
				{
					
					numbers.push_back(getNumber(pattern, index));
					if(!iWantValues && numbers.back() == "")
					{
						cout << "Error unknown sign " << pattern[index] << " after ( in alternating argument!" << endl;
						return false;
					}
					else
					{
						for(int j=0; j<2; j++)
						{
							while(pattern[index] == ' ')
								index++;
							if(!iWantValues && pattern[index] != ARG_SEPARATOR)
							{
								cout << "Error unknown sign " << pattern[index] << " after (number in alternating argument!" << endl;
								return false;
							}
							index++;
							while(pattern[index] == ' ')
								index++;
							numbers.push_back(getNumber(pattern, index));
							if(!iWantValues && numbers.back() == "")
							{
								cout << "Error not a number in () in alternating argument!" << endl;
								return false;
							}
						}
						if(!iWantValues && pattern[index] != ')')
						{
							cout << "Error no closing bracket ) in alternating argument!" << endl;
							return false;
						}
						//we need to check if the values inserted are correct
						double change = atof(numbers[2].c_str());
						double to = atof(numbers[1].c_str());
						double from = atof(numbers[0].c_str());
						if(!iWantValues && (from > to || change > (to - from)))
						{
							cout << "Error in values in alternating argument!" << endl;
							return false;
						}
					}
				}
			}
			else if (pattern[index] == '[')
			{
				index++;
				numbers.push_back(getNumber(pattern, index));
				if(!iWantValues && numbers.back() == "")
				{
					cout << "Error unknown sign " << pattern[index] << " after [ in alternating argument!" << endl;
					return false;
				}
				else if(!iWantValues && pattern[index] != ']')
				{
					cout << "Error unknown sign " << pattern[index] << " after [number in alternating argument!" << endl;
					return false;
				}
			}
			else if(!iWantValues)
			{
				cout << "Error unknown sign " << pattern[index] << " after Z in alternating argument!" << endl;
				return false;
			}

			if(iWantValues)
				enteredVals = numbers;
		}
	}
	
	return true;
}

vector<string> insertOrPromptForAlternatingArgs(string formula)
{
	vector<string> formulas;
	vector<string> values;

	while(isLetter(formula[gAlternatingArgPosition-1]) || isDigit(formula[gAlternatingArgPosition-1]) || formula[gAlternatingArgPosition-1] == ' ')
		gAlternatingArgPosition--;

	string argName = getNameWithSpaces(formula, gAlternatingArgPosition);

	checkAlternatingArgument(formula, gAlternatingArgPosition, values, true);

	if(values.size() == 0)		//:Z()
	{

	}
	else if(values.size() == 1) //:Z[a]
	{
		int amount = (int)atof(values[0].c_str());
		int argBegin = gAlternatingArgPosition - argName.size() - 4 /*:Z[]*/ - values[0].length() - 1 /*{*/;
		int argLength = gAlternatingArgPosition - argBegin + 2 /*}*/;
		for(int i = 0; i<amount; i++)
		{
			string formulaWithVal = formula;
			string toInsert;

			double x;
			cout << "Enter the value for " << argName << "[" << i << "]: ";
			cin >> x;
			cin.sync();

			toInsert = '(' + std::to_string(x) + ')'; //brackets in terms negative number
			formulaWithVal.replace(argBegin, argLength, toInsert);
			formulas.push_back(formulaWithVal);
		}
	}
	else						//:Z(a,b,c)
	{

	}

	return formulas;
}

//this function also inserts the argument number in functions with dynamic amount of arguments
bool isFormulaCorrect(string &formula)
{
	int openingBrackets = 0;
	int closingBrackets = 0;

	for(int i=0; i<formula.length(); ++i)
	{
		while(formula[i] == ' ')
			i++;

		if(formula[i] == '#')
		{
			i++;
			if(!checkConstantName(formula, i))
				return false;
		}

		if(formula[i] == '{')
		{
			openingBrackets++;
			i++;
			if(i >= formula.length() || formula[i] != '$')
			{
				cout << "Error no '$' sign in user argument!" << endl;
				return false;
			}
			if(!isLetter(formula[++i])) //auto-numering of arguments
			{
				string toInsert = std::to_string(openingBrackets);
				formula.insert(i, toInsert);
				i += toInsert.length();
			}
			else
				getNameWithSpaces(formula, i);

			if(!checkAlternatingArgument(formula, i))
				return false;
		}
		if(formula[i] == '}')
			closingBrackets++;

		if(isLetter(formula[i]))
		{
			if(!checkFunctionArgumentAmount(formula, i))
				return false;
		}
	}

	if(openingBrackets != closingBrackets)
	{
		cout << "Error wrong number of brackets in user arguments!" << endl;
		return false;
	}

	return true;
}

// substitutes constant names "#name" in string formula with their respective values
// described in the Constants singleton class
string insertConstantValues(string formula)
{
	string retval = "";
	for(int i = 0; i < formula.length();)
	{
		if(formula[i] == '#')
		{
			i++;

			string constName = getName(formula, i);

			retval += '(' + Constants::constantStringValue(constName) + ')'; // brackets in case the number is negative
		}
		else // not #
		{ 
			retval += formula[i++]; // incrementing i
		}
	}
	return retval;
}

// asks user for each parameter of the formula and returns a string
// where parameters "{$parameter_name}" are substituted with values given by the user
// USAGE NOTE: it should be checked beforehand whether the formula is correct or not
string getUserValues(string formula)
{ // TODO: array user parameters
	string retval = "";
	for(int i = 0; i < formula.length();)
	{
		if(formula[i] == '{')
		{
			i += 2; // go to the first character of parameter name
			string paramName = getNameWithSpaces(formula, i);
			if(formula[i] == ':')
			{
				// TODO: array argument. let's do it later
			}
			// prompt user for input
			cout << "Enter the value for " << paramName << ": ";
			double x;
			cin >> x;
			cin.sync(); // reset stream buffer (remove \n used to confirm number input) TODO: is there a smarter way to do it?
			retval += '(' + std::to_string(x) + ')';

			while(formula[i++]!='}'); // get to the next character after the closing bracket
		}
		else
		{
			retval += formula[i++];
		}
	}
	return retval;
}

// calculates function values and substitutes function calls with the values
/*string insertFunctionValues(string formula)
{
	string retval = "";
	for(int i = 0; i < formula.length();)
	{
		if(isLetter(formula[i])) // found first char of a function name
		{
			string functionName = "";
			vector<double> args;
			string tmpArg;

			while( formula[i] != '(' )
				functionName += formula[i++];
			
			while(formula[i]=='(')i++; // skip '('

			while( formula[i] != ')' )
			{
				if( formula[i] != ARG_SEPARATOR )
				{
					tmpArg += formula[i++];
				}else{ // formula[i] == ARG_SEPARATOR
					args.push_back(atof(tmpArg.c_str()));
					tmpArg = "";
					i++;
				}
			}			
			if(tmpArg.length() > 0)
				args.push_back(atof(tmpArg.c_str()));

			while(formula[i] == ')')i++; // skip ')'

			if(!Functions::functionExists(functionName,args.size())) // TODO: this should only be checked in isFormulaCorrect()
			{
				cout << "Function " << functionName << " with " << args.size() << " parameters is not valid.\n";
				return "";
			}

			retval.append('('+to_string(Functions::functionValue(functionName,args))+')');

		}else{
			retval += formula[i++];
		}
	}
	return retval;
};  */

vector<string> parseFormula(string formula)
{
	vector<string> formulas;

	if(gAlternatingArgPosition)
	{
		formulas = insertOrPromptForAlternatingArgs(formula);
	}
	else
		formulas.push_back(formula);

	for(auto it = formulas.begin(); it != formulas.end(); it++)
	{
		//get user arguments values
		*it = getUserValues(*it);
		//substitute constant names with values
		*it = insertConstantValues(*it);
	}


	return formulas;
}

#endif // !_PARSER_H_