// fmake.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

using namespace std;


class param {
public:
	regex reg;
	string name;
	string value;

	param(const string& name, const string& value)
		: name(name), value(value), reg(string("[\\u007B]{2}[\\s]*") + name + string("[\\s]*[\\u007D]{2}"))
	{}

	param(const param&) = default;

	param(param&&) = default;

	~param() = default;

	param& operator=(const param &) = default;

	param& operator=(param &&) = default;

	bool operator<(const param& p)const {
		return name < p.name;
	}

	bool operator!=(const param& p)const {
		return name != p.name;
	}
	
	bool operator==(const param& p)const {
		return name == p.name;
	}

	string toString() const {
		return name + ": " + value;
	}
};

class Resolver {
	const string DETECT_PARAM = "[\\u007B]{2}[\\s]*[a-zA-Z0-9_]+[\\s]*[\\u007D]{2}";
	const string PARAM_NAME = "[a-zA-Z0-9_-]+";
	const regex detParReg = regex(DETECT_PARAM);
	const regex parNamReg = regex(PARAM_NAME);

	map<string, param> params;

	class Element;

	typedef map < int, vector< Element* > > Tree;
	typedef map < string, Element > Elements;

	class Element {
		set<string> depNode;
		string name;
		int dep;
	public:

		Element(const set<string>& n, string name, int dep)
			: depNode(n), name(name), dep(dep)
		{}

		const string& getName() const {
			return name;
		}

		const set<string>& getDepNode() const {
			return depNode;
		}

		int deep(Elements& m, Tree &t);

		int deep() const {
			return dep;
		}
	};

public:
	Resolver() = default;

	Resolver(const Resolver&) = default;

	Resolver(Resolver&&) = default;

	~Resolver() = default;

	Resolver& operator=(const Resolver &) = default;

	Resolver& operator=(Resolver &&) = default;

	bool addParam(const string& name, const string& value) {
		auto iter = params.find(name);
		if (iter != params.end())
			return false;
		params.emplace(name,param(name, value));
		return true;
	}

	param& getParam(string key);

	bool resolveParams();
};

param& Resolver::getParam(string key) {
	auto iter = params.find(key);
	if (iter == params.end())
		throw exception("Find no defined params!");
	return params.at(key);
}

bool Resolver::resolveParams() {
	Elements mapa;
	Tree lvl;

	for (auto& p : params) {
		smatch n;
		set<string> dependecies;

		string text = p.second.value;
		while (regex_search(text, n, detParReg)) {
			smatch m2;
			string sufix(n.str());
			if (regex_search(sufix, m2, parNamReg)) {
				std::string s = m2.str();
				dependecies.emplace(move(s));
			}
			text = n.suffix();
		}

		mapa.emplace(make_pair(p.first, Element(dependecies, p.first, 0)));
	}

	int maxDeep = 0;
	for (auto& m : mapa) {
		maxDeep = max(maxDeep, m.second.deep(mapa, lvl));
	}

	for (auto& l : lvl) {
		if (l.first == 1)
			continue;
		for (auto& e : l.second) {
			auto& param = getParam(e->getName());
			for (auto& t : e->getDepNode()) {
				auto temp = getParam(t);
				param.value = regex_replace(param.value,temp.reg, temp.value);
			}
			
		}
	}

	return maxDeep;
}

int Resolver::Element::deep(Resolver::Elements& m, Tree &t) {
	if (dep > 0)
		return dep;

	if (dep == -1)
		throw exception("Detected circle in params list!");

	dep = -1;
	int tmpDeep = 0;
	for (auto& v : depNode) {
		auto iter = m.find(v);
		if (iter != m.end())
			tmpDeep = max(tmpDeep, iter->second.deep(m, t));
		else
			tmpDeep = max(tmpDeep, 0);
	}
	dep = tmpDeep + 1;
	t[dep].push_back(this);
	return dep;
}

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

	bool addParam(const string& name, size_t n, OpcjeParametru::Function f) {
		params.emplace(name, OpcjeParametru(n, f));
		return true;
	}

private:
	std::map< std::string, OpcjeParametru> params;
};

int main(unsigned int argv, char* argc[]) {
	Args arg; 
	Resolver resolver;
	string infileName;
	string outfileName;

	arg.addParam("-f", 1, [&](std::vector<char*> l)->bool {
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

	arg.addParam("-o", 1, [&](std::vector<char*> l)->bool {
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

	arg.process(argv, argc);

	resolver.addParam("imie", "Daniel");
	resolver.addParam("powitanie", "Witaj {{ imie }}");
	resolver.addParam("a", "{{ c }}i");
	resolver.addParam("b", "{{ c }}{{ a}}");
	resolver.addParam("c", "1");

	resolver.resolveParams();
	
	cout << infileName << endl;
	cout << outfileName << endl;
    return 0;
}
