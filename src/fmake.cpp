// fmake.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

using namespace std;

int main()
{
	string parametr;
	getline(cin, parametr);
	cout << parametr << endl;

	regex reg("[\\u007B]{2}[\\s]+imie[\\s]+[\\u007D]{2}");
	string replaced = regex_replace(parametr, reg, "Daniel");
	cout << replaced << endl;
    return 0;
}
