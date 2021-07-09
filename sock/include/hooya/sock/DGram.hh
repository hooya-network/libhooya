#pragma once

#include <vector>
#include <string_view>
#include <stdexcept>

namespace hooya::sock {
typedef struct DGramHeader {
public:
	uint32_t DataLen : 32;
	uint32_t DataOffset : 32;
} DGramHeader_t;

class DGram {
public:
	/**
	 * Construct a datagram from a raw stream of network-order characters
	 * \param d Raw network-order data from a socket
	 * \param len Length of data
	 */
	bool Parse(const char *d, size_t len);
private:
	/**
	 * Datagram header comes first in the datagram and describes the payload and
	 * sending context
	 */
	DGramHeader_t header;

	/**
	 * The datagram payload comes after the header
	 */
	std::vector<std::byte> payload;
}; }
