#pragma once
#include "stdafx.h"

class Args {
public:
	struct OpcjeParametru {
		typedef std::function<bool(std::vector<char*>)> Function;
		OpcjeParametru(size_t n, Function f)
			: n(n), f(f) {}

		size_t n = 0;
		Function f;
	};

	Args() = default;

	~Args() = default;

	bool process(unsigned int argv, char * argc[]) {
		if (!argc || argv <= 0)
			return false;

		for (unsigned int numer = 0; numer < argv; ++numer) {
			if (!argc[numer])
				continue;
			std::string argument(argc[numer]);
			if (argument.empty())
				continue;
			auto parametr = params.find(argument);
			if (parametr == params.end())
				continue;
			if (parametr->second.n + numer >= argv)
				return false;
			std::vector<char*> lista;
			lista.reserve(parametr->second.n);
			for (unsigned char offset = 1; offset <= parametr->second.n; ++offset) {
				lista.push_back(argc[numer + offset]);
			}
			if (parametr->second.f(lista))
				numer += parametr->second.n;
			else
				return false;
		}
		return true;
	}

	bool addParam(const std::string& name, size_t n, OpcjeParametru::Function f) {
		params.emplace(name, OpcjeParametru(n, f));
		return true;
	}

private:
	std::map< std::string, OpcjeParametru> params;
};
