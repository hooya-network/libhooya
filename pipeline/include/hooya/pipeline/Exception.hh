#pragma once

#include <stdexcept>

namespace hooya::pipeline::exception {
class Interrupt : public std::exception {
public:
	Interrupt(const char *desc);
	virtual char const *what() const noexcept { return d; }
private:
	const char *d;
}; }
