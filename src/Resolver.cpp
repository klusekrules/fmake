#include "stdafx.h"
#include "Resolver.h"

Resolver::Resolver(std::ostream & out, std::istream & in)
	: out(out), in(in)
{
}

bool Resolver::addParam(const std::string& name, const std::string& value) {
	auto iter = find(params.begin(), params.end(), Param(name, ""));
	if (iter != params.end())
		return false;
	params.emplace_back(Param(name, value));
	return true;
}

Param& Resolver::getParam(std::string key) {
	auto iter = find(params.begin(), params.end(), Param(key, ""));
	if (iter == params.end()) {
		if (askAboutParam) {
			createParam(key);
			return params.back();
		}
		else {
			throw std::exception("Find no defined params!");
		}
	}
	return *iter;
}

bool Resolver::createParam(std::string key) {
	std::string value;
	out << "Parameter " << key << ":";
	std::getline(in, value);
	return addParam(key, value);
}

std::set<std::string> Resolver::resolveDependecies(const std::string &p) {
	std::smatch n;
	std::set<std::string> dependecies;

	std::string text = p;
	while (regex_search(text, n, detParReg)) {
		std::smatch m2;
		std::string sufix(n.str());
		if (regex_search(sufix, m2, parNamReg)) {
			std::string s = m2.str();
			dependecies.emplace(getParam(s).name); // Get or create
		}
		text = n.suffix();
	}

	return std::move(dependecies);
}

bool Resolver::resolveParams() {
	Elements mapa;
	Tree lvl;

	for (auto& p : params) {
		mapa.emplace(make_pair(p.name, Element(std::move(resolveDependecies(p.value)), p.name, 0)));
	}

	int maxDeep = 0;
	for (auto& m : mapa) {
		maxDeep = std::max(maxDeep, m.second.deep(mapa, lvl));
	}

	for (auto& l : lvl) {
		if (l.first == 1)
			continue;
		for (auto& e : l.second) {
			auto& param = getParam(e->getName());
			for (auto& t : e->getDepNode()) {
				auto temp = getParam(t);
				param.value = regex_replace(param.value, temp.reg, temp.value);
			}

		}
	}

	return maxDeep;
}

int Resolver::Element::deep(Resolver::Elements& m, Tree &t) {
	if (dep > 0)
		return dep;

	if (dep == -1)
		throw std::exception("Detected circle in params list!");

	dep = -1;
	int tmpDeep = 0;
	for (auto& v : depNode) {
		auto iter = m.find(v);
		if (iter != m.end())
			tmpDeep = std::max(tmpDeep, iter->second.deep(m, t));
		else
			tmpDeep = std::max(tmpDeep, 0);
	}
	dep = tmpDeep + 1;
	t[dep].push_back(this);
	return dep;
}

void Resolver::resolveStream(std::istream& in, std::ostream& out) {
	std::smatch n;
	std::string str;
	bool found;

	do {
		std::getline(in, str);
		found = false;
		while (regex_search(str, n, detParReg)) {
			found = true;
			out << n.prefix();
			std::smatch m2;
			std::string param(n.str());
			if (regex_search(param, m2, parNamReg)) {
				std::string s = m2.str();
				auto& p = getParam(s);
				out << p.value;
			}
			else {
				out << param;
			}
			str = n.suffix();
		}

		if (!found) {
			out << str;
		}

		if (in.good()) {
			out << std::endl;
		}
	} while (in.good());

}
