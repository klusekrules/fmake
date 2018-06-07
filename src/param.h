#pragma once
#include <string>
#include <regex>

class param {
public:
	std::regex reg;
	std::string name;
	std::string value;

	param(const std::string& name, const std::string& value)
		: name(name), value(value), reg(std::string("[\\u007B]{2}[\\s]*") + name + std::string("[\\s]*[\\u007D]{2}"))
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

	std::string toString() const {
		return name + ": " + value;
	}
};