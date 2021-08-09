#pragma once

#include <vector>
#include <string>

namespace hooya::crypto {
class Bin2 {
public:
	/**
	 * Hexadecimal (base-16) representation of binary data
	 * \param bin Raw binary data
	 * \param uppercase Uppercase hex letters
	 * \return Base-16 string
	 */
	static std::string Base16(const std::vector<uint8_t> &bin, bool uppercase = false);

	/**
	 * Base-64 representation of binary data
	 * \param bin Raw binary data
	 * \return Base-64 string
	 */
	static std::string Base64(const std::vector<uint8_t> &bin);

	/**
	 * Base-8 representation of binary data
	 * \param bin Raw binary data
	 * \return Base-8 string
	 */
	static std::string Base8(const std::vector<uint8_t> &bin);
private:

	/**
	 * Transform a number 0 ~ 15 into a hex digit
	 * \param one One number 0 ~ 15
	 * \param uppercase Uppercase hex letters
	 * \return Hex character 0 ~ F
	 */
	static char hexDigit(uint8_t one, bool uppercase);
}; }
