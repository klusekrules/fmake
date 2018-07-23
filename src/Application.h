#pragma once
#include "Args.h"
#include "Resolver.h"
#include <fstream>

class Application {
private:
	Args arguments;
	Resolver resolver;
	std::string infileName;
	std::string outfileName;
	std::string paramsFile;
	std::ifstream inStream;
	std::ofstream outStream;

	void initArgs();

	void afterProcessArgs();
	
public:

	Application(unsigned int argv, char* argc[])
	{
		initArgs();
		arguments.process(argv, argc);
		afterProcessArgs();
	}

	int process();

	Application(const Application&) = default;

	Application(Application&&) = default;

	~Application() = default;

	Application& operator=(const Application &) = default;

	Application& operator=(Application &&) = default;

};