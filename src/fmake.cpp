// fmake.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

using namespace std;

class parametr {
private:
	regex reg;
	string name;
	string value;

public:
	parametr(const string& name, const string& value)
		: name(name), value(value), reg(string("[\\u007B]{2}[\\s]+") + name + string("[\\s]+[\\u007D]{2}"))
	{}

	parametr(const parametr&) = default;

	parametr(parametr&&) = default;

	~parametr() = default;

	parametr& operator=(const parametr &) = default;

	parametr& operator=(parametr &&) = default;

	void prepare_value(const parametr& p) {
		value = regex_replace(value, p.reg, p.value);
	}

	bool operator!=(const parametr& p)const {
		return name != p.name;
	}
	
	bool operator==(const parametr& p)const {
		return name == p.name;
	}

	string toString() const {
		return name + ':' + value;
	}
};

struct OpcjeParametru {
	OpcjeParametru(unsigned char iloscParametrow, std::function<bool(std::vector<char*>)> funkcja)
		: iloscParametrow_(iloscParametrow), funkcja_(funkcja) {}

	unsigned char iloscParametrow_ = 0;
	std::function<bool(std::vector<char*>)> funkcja_;
};

bool przetworzArgumenty(int argv, char * argc[], const std::map< std::string, OpcjeParametru>& params) {
	if (!argc || argv <= 0)
		return false;

	for (int numer = 0; numer < argv; ++numer) {
		if (!argc[numer])
			continue;
		std::string argument(argc[numer]);
		if (argument.empty())
			continue;
		auto parametr = params.find(argument);
		if (parametr == params.end())
			continue;
		if (parametr->second.iloscParametrow_ + numer >= argv)
			return false;
		std::vector<char*> lista;
		lista.reserve(parametr->second.iloscParametrow_);
		for (unsigned char offset = 1; offset <= parametr->second.iloscParametrow_; ++offset) {
			lista.push_back(argc[numer + offset]);
		}
		if (parametr->second.funkcja_(lista))
			numer += parametr->second.iloscParametrow_;
		else
			return false;
	}
	return true;
}

int main( int argv, char* argc[] ) {

	std::map< std::string, OpcjeParametru> startParams;
	vector<parametr> listOfParams;
	string fileName;

	startParams.emplace("-f", OpcjeParametru(1, [&](std::vector<char*> lista)->bool {
		if (lista.size() == 0)
			return false;

		auto wsk = lista.begin();
		if (*wsk) {
			std::string nazwa(*wsk);
			if (!nazwa.empty()) {
				fileName = nazwa;
				return true;
			}
		}
		return false;
	}));

	przetworzArgumenty(argv, argc, startParams);

	listOfParams.emplace_back("imie", "Daniel");
	listOfParams.emplace_back("powitanie", "Witaj {{ imie }}");

	for (auto& p: listOfParams) {
		for (auto& r: listOfParams) {
			if (p!=r)
				p.prepare_value(r);
		}
	}

	for (auto& p : listOfParams) {
		cout << p.toString() << endl;
	}

	cout << argv << endl;

	for (size_t i = 0; i < argv; i++) {
		cout << argc[i] << endl;
	}

	cout << fileName << endl;
    return 0;
}
