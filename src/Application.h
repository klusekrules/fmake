#pragma once
#include "Args.h"
#include "Resolver.h"

class Application {
private:
	Args arguments;
	Resolver resolver;
	std::string infileName;
	std::string outfileName;

	void initArgs();
	
public:

	Application(unsigned int argv, char* argc[])
	{
		initArgs();
		arguments.process(argv, argc);
	}

	int process();

	Application(const Application&) = default;

	Application(Application&&) = default;

	~Application() = default;

	Application& operator=(const Application &) = default;

	Application& operator=(Application &&) = default;

};