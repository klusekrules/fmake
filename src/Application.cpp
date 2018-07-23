#include "stdafx.h"
#include "Application.h"

void Application::initArgs() {
	arguments.addParam("-f", 1, [&](std::vector<char*> l)->bool {
		if (l.size() == 0)
			return false;

		auto wsk = l.begin();
		if (*wsk) {
			std::string nazwa(*wsk);
			if (!nazwa.empty()) {
				infileName = nazwa;
				return true;
			}
		}
		return false;
	});

	arguments.addParam("-o", 1, [&](std::vector<char*> l)->bool {
		if (l.size() == 0)
			return false;

		auto wsk = l.begin();
		if (*wsk) {
			std::string nazwa(*wsk);
			if (!nazwa.empty()) {
				outfileName = nazwa;
				return true;
			}
		}
		return false;
	});

	arguments.addParam("-p", 1, [&](std::vector<char *> l)->bool {
		if (l.size() == 0)
			return false;
		auto wsk = l.begin();
		if (*wsk) {
			std::string nazwa(*wsk);
			if (!nazwa.empty()) {
				paramsFile = nazwa;
				return true;
			}
		}
		return false;
	});
}

void Application::afterProcessArgs() {
	if (!infileName.empty() && !outfileName.empty()) {
		inStream.open(infileName, std::ios::in);
		outStream.open(outfileName, std::ios::trunc);
	}

	if (!paramsFile.empty()) {
		std::ifstream pStr(paramsFile, std::ios::in);
		std::string line;

		do {
			std::getline(pStr, line);
			auto pos = line.find_first_of('=');
			if (pos != std::string::npos) {
				resolver.addParam(line.substr(0, pos), line.substr(pos + 1));
			}
		} while (pStr.good());
	}
}

int Application::process() {
	resolver.resolveParams();
	resolver.resolveStream(inStream, outStream);
	return 0;
}