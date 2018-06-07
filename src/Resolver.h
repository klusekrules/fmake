#pragma once
#include "Param.h"

class Resolver {
	const std::string DETECT_PARAM = "[\\u007B]{2}[\\s]*[a-zA-Z0-9_]+[\\s]*[\\u007D]{2}";
	const std::string PARAM_NAME = "[a-zA-Z0-9_-]+";
	const std::regex detParReg = std::regex(DETECT_PARAM);
	const std::regex parNamReg = std::regex(PARAM_NAME);

	std::map<std::string, Param> params;

	class Element;

	typedef std::map < int, std::vector< Element* > > Tree;
	typedef std::map < std::string, Element > Elements;

	class Element {
		std::set<std::string> depNode;
		std::string name;
		int dep;
	public:

		Element(const std::set<std::string>& n, std::string name, int dep)
			: depNode(n), name(name), dep(dep)
		{}

		const std::string& getName() const {
			return name;
		}

		const std::set<std::string>& getDepNode() const {
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

	bool addParam(const std::string& name, const std::string& value) {
		auto iter = params.find(name);
		if (iter != params.end())
			return false;
		params.emplace(name, Param(name, value));
		return true;
	}

	Param& getParam(std::string key);

	bool resolveParams();
};