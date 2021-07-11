#pragma once

#include <vector>
#include <string_view>
#include <stdexcept>

namespace hooya::sock {
/**
 * Header describing the context of the payload within the larger transaction
 */
typedef struct DGramHeader {
public:
	/**
	 * Magic number
	 */
	uint32_t Magic : 32;

	/**
	 * Datagram format version
	 */
	uint16_t Version : 16;

	/**
	 * Reserved
	 */
	uint16_t Reserved : 16;

	/**
	 * Length of the transaction of which this payload is a part of
	 */
	uint32_t ContextLen : 32;

	/**
	 * Offset of the payload within the larger transaction stream
	 */
	uint32_t DataOffset : 32;
} DGramHeader_t;

class DGram {
public:
	/**
	 * Construct a datagram from a raw stream of network-order characters
	 * \param d Raw network-order data from a socket
	 * \param len Length of data
	 */
	bool Parse(const uint8_t *d, size_t len);

	bool Parse(const std::vector<uint8_t> &d);

	/**
	 * Sets the payload of this DGram (for sending)
	 * \param p Payload data
	 *
	 * TODO Would this make more sense to pass in as a stream?
	 */
	void Payload(const std::vector<uint8_t> &p);

	void Payload(const std::string &s);

	std::vector<uint8_t> Payload();

	/**
	 * Sets the header information conveyed in this DGram (for sending)
	 * \param offset Offset of the payload in the context of the data stream
	 * \param contextlen Combined length of all payloads of packets in this
	 * transaction
	 */
	void Header(int offset, int contextlen);
private:
	/**
	 * Datagram header comes first in the datagram and describes the payload and
	 * sending context
	 */
	DGramHeader_t header;

	/**
	 * The datagram payload comes after the header
	 */
	std::vector<uint8_t> payload;

	/**
	 * Magic header number
	 */
	const uint32_t MAGIC = 0x68596121;

	const uint32_t CURRENTVERSION = 0x00;
}; }
