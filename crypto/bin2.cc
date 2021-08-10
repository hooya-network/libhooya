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
}

std::string Bin2::Base8(const std::vector<uint8_t> &bin) {
	std::string ret;

	/* 3-bit accumulator for a single octal digit */
	uint8_t octalDigit = 0;

	/* Accumulated bits in octalDigit */
	size_t currWidth = 0;

	/* Current bit of given byte - left-to-right so start from 7 */
	uint8_t currBit = 7;

	auto currByte = bin.begin();

	for (currByte = bin.begin(); currByte != bin.end(); ) {
		/* Construct octal digit bit-by-bit */
		octalDigit = (octalDigit << 1) | (((1 << currBit) & *currByte) >> currBit);

		/* Octal digit has 3 bits - advance to next one */
		if (++currWidth > 2) {
			/* Append ASCII encoding of octalDigit to string */
			ret += octalDigit + '0';

			/* Next octalDigit */
			currWidth = octalDigit = 0;
		}

		/* End of current byte - advance to next one */
		if (--currBit > 7) {
			currBit = 7;
			currByte++;
		}
	}

	return ret;
}

std::string Bin2::Base64(const std::vector<uint8_t> &bin) {
	std::string ret;
	uint8_t digit;

	/* Convert to octal first */
	std::string octal = Base8(bin);

	/* Ensure an even number of octal digits */
	if (octal.size() & 1)
		octal = '0' + octal;

	/* Process octal digits as pairs (2, 8-bit numbers) -> 1, 64-bit number */
	for (auto i = octal.begin(); i < octal.end(); i += 2) {
		/* First octal digit */
		digit = (*i - '0') * 8;

		/* ... plus second octal digit */
		digit += *(i + 1) - '0';

		/* Encode decimal as the normal base-64 character */
		ret += BASE64_TABLE[digit];
	}

	if (ret.size() % 4)
		/* Padding */
		ret += std::string(4 - (ret.size() % 4), '=');

	return ret;
}

const char Bin2::BASE64_TABLE[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
	'O' , 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b',
	'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
	'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '+', '/'
};

}
