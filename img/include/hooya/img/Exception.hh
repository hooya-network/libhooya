#pragma once

#include <stdexcept>

namespace hooya::img::exception {
class FileRead : public std::exception {
public:
	FileRead(const char *desc);
	virtual char const *what() const noexcept { return d; }
private:
	const char *d;
}; }
