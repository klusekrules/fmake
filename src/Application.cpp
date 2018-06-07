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
}

int Application::process() {
	resolver.addParam("imie", "Daniel");
	resolver.addParam("powitanie", "Witaj {{ imie }}");
	resolver.addParam("a", "{{ c }}i");
	resolver.addParam("b", "{{ c }}{{ a}}");
	resolver.addParam("c", "1");

	resolver.resolveParams();

	return 0;
}