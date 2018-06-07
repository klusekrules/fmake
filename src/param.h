#pragma once
#include <string>
#include <regex>

class Param {
public:
	std::regex reg;
	std::string name;
	std::string value;

	Param(const std::string& name, const std::string& value)
		: name(name), value(value), reg(std::string("[\\u007B]{2}[\\s]*") + name + std::string("[\\s]*[\\u007D]{2}"))
	{}

	Param(const Param&) = default;

	Param(Param&&) = default;

	~Param() = default;

	Param& operator=(const Param &) = default;

	Param& operator=(Param &&) = default;

	bool operator<(const Param& p)const {
		return name < p.name;
	}

	bool operator!=(const Param& p)const {
		return name != p.name;
	}

	bool operator==(const Param& p)const {
		return name == p.name;
	}

	std::string toString() const {
		return name + ": " + value;
	}
};
