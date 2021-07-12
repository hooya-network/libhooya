#pragma once

#include <vector>
#include <stdexcept>

namespace hooya::sock {
/**
 * \typedef DGramHeader_t
 * Header describing the context of the payload within the larger transaction
 *
 * \note All fields are represented in network-order
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

/**
 * Datagram representation. Each packet encapsulates a payload, and many
 * payloads may be combined to form a full message / context
 */
class DGram {
public:
	/**
	 * Construct a datagram from a raw stream of network-order characters
	 * \param d Raw network-order data from a socket
	 * \param len Length of data
	 * \TODO Could this be done with a data stream?
	 */
	bool Parse(const uint8_t *d, size_t len);

	/**
	 * Construct a datagram from a raw sequence of bytes
	 * \param d Raw network-order data from a socket
	 */
	bool Parse(const std::vector<uint8_t> &d);

	/**
	 * Sets the payload of this DGram (for sending)
	 * \param p Payload data
	 *
	 * \TODO Would this make more sense to pass in as a stream?
	 */
	void Payload(const std::vector<uint8_t> &p);

	/**
	 * Sets the payload of this DGram (for sending)
	 * \param s Payload data as a string
	 */
	void Payload(const std::string &s);

	/**
	 * Copy of the internal payload
	 *
	 * \return Packet payload
	 */
	std::vector<uint8_t> Payload() const;

	/**
	 * Raw representation of this datagram suitable for and egress buffer
	 *
	 * \return Contiguous representation of raw packet contents
	 */
	std::vector<uint8_t> Raw() const;

	/**
	 * Sets the header information conveyed in this DGram (for sending)
	 * \param offset Offset of the payload in the context of the data stream
	 * \param contextlen Combined length of all payloads of packets in this
	 * transaction
	 */
	void Header(uint32_t offset, uint32_t contextlen);

	/**
	 * Construct a one-off / singleton packet with the given payload
	 * \param p Payload data
	 */
	void Singleton(const std::vector<uint8_t> &p);

	/**
	 * Magic header number
	 */
	static const uint32_t MAGIC = 0x68596121;

	/**
	 * Current revision of encapsulating datagram format
	 */
	static const uint32_t CURRENTVERSION = 0x00;

	/**
	 * Offset of the payload in singleton packets is zero
	 */
	static const size_t NO_OFFSET = 0;
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
}; }
