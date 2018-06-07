#include "stdafx.h"
#include "Resolver.h"

Param& Resolver::getParam(std::string key) {
	auto iter = params.find(key);
	if (iter == params.end())
		throw std::exception("Find no defined params!");
	return params.at(key);
}

bool Resolver::resolveParams() {
	Elements mapa;
	Tree lvl;

	for (auto& p : params) {
		std::smatch n;
		std::set<std::string> dependecies;

		std::string text = p.second.value;
		while (regex_search(text, n, detParReg)) {
			std::smatch m2;
			std::string sufix(n.str());
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
