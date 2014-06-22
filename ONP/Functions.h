#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include <vector>
#include <map>
#include <math.h>
#include <time.h>
using namespace std;

double addFunction(vector<double> args)
{
	return args[0] + args[1];
}

// returns args[0] - args[1]
double subtractFunction(vector<double> args)
{
	return args[0] - args[1];
}

double multiplyFunction(vector<double> args)
{
	return args[0] * args[1];
}

double divideFunction(vector<double> args)
{
	return args[0] / args[1];
}

double divideIntFunction(vector<double> args)
{
	return (double)((int)((int)args[0] / (int)args[1]));
}

double moduloFunction(vector<double> args)
{
	return (int)args[0] % (int)args[1];
}

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

double factorialFunction(vector<double> args)
{	
	args[0] = (double)((int)args[0]);
	if(args[0] == 1 || args[0] == 0)
		return 1;
	else
	{
		double a = args[0];
		args[0]--;
		return a * factorialFunction(args);
	}
}

#endif // !_FUNCTIONS_H_
