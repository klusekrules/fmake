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

int main() {
	vector<parametr> listOfParams;

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

    return 0;
}
