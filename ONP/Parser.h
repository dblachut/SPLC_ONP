#ifndef _PARSER_H_
#define _PARSER_H_
#include "Logic.h"
#include "Functions.h"

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
					else
						argEntered++; //functon as an argment
				}
				else if(pattern[index] == '#')
				{
					index++;
					if(!checkConstantName(pattern, index))
						return false;
					argEntered++; //const as an argument
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
			if(i+1 >= formula.length() || formula[i+1] != '$')
			{
				cout << "Error no '$' sign in user argument!" << endl;
				return false;
			}
		}
		else if(formula[i] == '}')
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

	cout << "Potrzebujemy: " << openingBrackets << " argumentow uzytkownika." << endl;

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

string parseFormula(string formula)
{
	formula = getUserValues(formula);
	//substitute constant names with values
	formula = insertConstantValues(formula);

	return formula;
}

#endif // !_PARSER_H_