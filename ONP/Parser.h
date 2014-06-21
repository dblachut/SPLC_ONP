#ifndef _PARSER_H_
#define _PARSER_H_
#include "Logic.h"
#include "Functions.h"


bool isFormulaCorrect(string formula)
{
	int openingBrackets = 0;
	int closingBrackets = 0;

	for(int i=0; i<formula.length(); ++i)
	{
		while(formula[i] == ' ')
			i++;

		if(formula[i] == '#')
		{
			if(i+1 >= formula.length() || !isLetter(formula[i+1]))
			{
				cout << "Error brak identyfikatora stalej!" << endl;
				return false;
			}

			int j = i+1;
			string constName = "";
			while((j < formula.length()) && (isLetter(formula[j]) || isDigit(formula[j])))
				constName += formula[j++];
			if( !Constants::inst.constantExists(constName) ){
				cout << "Error stala " << constName << " nie istnieje!" << endl;
				return false;
			}
		}

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




// substitutes constant names "#name" in string formula with their respective values
// described in the Constants singleton class
string insertConstantValues(string formula){
	string retval = "";
	for(int i = 0; i < formula.length();){
		if(formula[i] == '#')
		{						
			i++; // go to the next character after '#'
			string constName = "";
			while((i < formula.length()) && (isLetter(formula[i]) || isDigit(formula[i])))
				constName += formula[i++]; // incrementing i and adding one character to constNames until non-alphanumeric character found
			if( !Constants::inst.constantExists(constName) ){
				cout << "Error: constant " << constName << " is invalid!" << endl;
				break;
			}else{
				retval.append('('+std::to_string(Constants::inst.constantValue(constName))+')'); // brackets in case the number is negative
			}
		}else{ // not #
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
			if(i+1 >= formula.length() || formula[i+1] != '$')
			{
				cout << "Error brak znaku $ w argumencie!" << endl;
				return "";
			}
			i += 2; // go to the first character of parameter name
			string paramName = "";
			while((i < formula.length()) && (isLetter(formula[i]) || isDigit(formula[i])))
				paramName += formula[i++];
			if(formula[i] == ':')
			{
				// TODO: array argument. let's do it later
			}
			// prompt user for input
			cout << "Enter the value for " << paramName << ": ";
			double x;
			cin >> x;
			cin.sync(); // reset stream buffer (remove \n used to confirm number input) TODO: is there a smarter way to do it?
			retval.append('('+std::to_string(x)+')');

			while(formula[i++]!='}'); // get to the next character after the closing bracket

		}else{
			retval += formula[i++];
		}
	}
	return retval;
}

// TODO: recursive function calls sin(sin(#x)) and sin(1-2) or sin(#pi/2)
// calculates function values and substitutes function calls with the values
string insertFunctionValues(string formula)
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
				if( formula[i] != ',' )
				{
					tmpArg += formula[i++];
				}else{ // formula[i] == ','
					args.push_back(atof(tmpArg.c_str()));
					tmpArg = "";
					i++;
				}
			}			
			if(tmpArg.length() > 0)
				args.push_back(atof(tmpArg.c_str()));

			while(formula[i] == ')')i++; // skip ')'

			if(!Functions::inst.functionExists(functionName,args.size())) // TODO: this should only be checked in isFormulaCorrect()
			{
				cout << "Function " << functionName << " with " << args.size() << " parameters is not valid.\n";
				return "";
			}

			retval.append('('+to_string(Functions::inst.functionValue(functionName,args))+')');

		}else{
			retval += formula[i++];
		}
	}
	return retval;
};  


string parseFormula(string formula)
{
	//substitute constant names with values
	formula = insertConstantValues(formula);

	// substitute functions with their calculated values NOTE: user arguments need to be filled out here already.
	// At this point, functions are the only texts(non numeric) in the formula
	formula = insertFunctionValues(formula);

	if(!isEquationCorrect(formula)){
		cout << "Incorrect equation\n";
		return "";
	}else{
		cout << "Equation correct\n";
	}
	return formula;
}

#endif // !_PARSER_H_