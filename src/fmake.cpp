// fmake.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Application.h"

int main(unsigned int argv, char* argc[]) {
	Application app(argv, argc);
	return app.process();
}
