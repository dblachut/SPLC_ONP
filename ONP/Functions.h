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

	static bool functionExists(string name)
	{
		if(inst.functions.find(name)  == inst.functions.end())
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
		double (*func)(vector<double>) = inst.functions[name].first;
		int argc = inst.functions[name].second;
		if(arguments.size() != argc)
			cout << "Error, not enough arguments for function call: " << name << endl;
		return func(arguments);
	}

	static vector<string> getNames()
	{
		vector<string> ret;
		for(map<string,pair<double (*)(vector<double>),int>>::iterator iter = inst.functions.begin(); iter != inst.functions.end(); ++iter)		
			ret.push_back(iter->first);
		return ret;
	}

private:
	map<string,pair<double (*)(vector<double>),int>> functions;

	Functions()
	{	// TODO: more
		functions["sin"] = make_pair(sinFunction,1);
		functions["cos"] = make_pair(cosFunction,1);
		functions["sqrt"] = make_pair(sqrtFunction,1);
		functions["pow"] = make_pair(powFunction,2);
		functions["rand"] = make_pair(randFunction,0);
	}
};  

Functions Functions::inst = Functions();



#endif // !_FUNCTIONS_H_
