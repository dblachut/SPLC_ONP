#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include <vector>
#include <map>
#include <math.h>
#include <time.h>
using namespace std;


double randFunction(vector<double> args)
{
	static bool seedon = false;
	if(!seedon)
	{
		srand(time(NULL));
		seedon = true;
	}
	return rand();
}

double powFunction(vector<double> args)
{
	return pow(args[0],args[1]);
}

double sinFunction(vector<double> args)
{
	return sin(args[0]);
}

double cosFunction(vector<double> args)
{
	return cos(args[0]);
}

double sqrtFunction(vector<double> args)
{
	return sqrt(args[0]);
}

class Functions
{
public:
	static Functions inst;		// instance

	static bool functionExists(string name, int argc)
	{
		if(inst.functions.find(make_pair(name,argc))  == inst.functions.end())
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	static double functionValue(string name, vector<double> arguments)
	{
		return inst.functions[make_pair(name,arguments.size())](arguments);
	}

private:
	map<pair<string,int>,double (*)(vector<double>)> functions;

	Functions()
	{	// TODO: more
		functions[make_pair("sin",1)] = sinFunction;
		functions[make_pair("cos",1)] = cosFunction;
		functions[make_pair("sqrt",1)] = sqrtFunction;
		functions[make_pair("pow",2)] = powFunction;
		functions[make_pair("rand",0)] = randFunction;
	}
};  

Functions Functions::inst = Functions();



#endif // !_FUNCTIONS_H_
