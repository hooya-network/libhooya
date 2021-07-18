#pragma once

#include <stdexcept>

namespace hooya::sock {
class BindException : public std::exception {
public:
	BindException(const char *desc);
	BindException(int eno);
	virtual char const *what() const noexcept { return d; }
private:
	const char *d;
};

class ParseException : public std::exception {
public:
	ParseException(const char *desc);
	virtual char const *what() const noexcept { return d; }
private:
	const char *d;
};

class Timeout : public std::exception {
public:
	Timeout(const char *desc);
	virtual char const *what() const noexcept { return d; }
private:
	const char *d;
};

class ShutdownException : public std::exception {
}; }

