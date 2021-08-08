#include "bin2.hh"

#include <cassert>
#include <iostream>

namespace hooya::crypto {
char Bin2::hexDigit(uint8_t one, bool uppercase) {
	if (one < 10)
		return one + '0';
	if (one < 16)
		return one + (uppercase ? 'A' : 'a') - 10;

	/* Not a hex digit, there's an error in your code (｀・ω・´)” */
	assert(false);
}

std::string Bin2::Base16(const std::vector<uint8_t> &bin, bool uppercase) {
	std::string ret;

	for (const auto &b : bin) {
		/* First hex character of a byte */
		ret += hexDigit(b >> 4, uppercase);
		/* Second hex character of a byte */
		ret += hexDigit((uint8_t)(b << 4) >> 4, uppercase);
	}

	return ret;
} }
