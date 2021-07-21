#include "hooya/sock/DGram.hh"

#include <arpa/inet.h>
#include <cstring>
#include "hooya/sock/Exception.hh"

namespace hooya::sock {
bool DGram::Parse(const uint8_t *d, size_t len) {
	const size_t pOffset = sizeof(DGramHeader_t);

	// Header cannot be larger than the datagram itself lol
	if (pOffset >= len)
		throw ParseException("Datagram too small to parse");

	// Extract header and payload raw data
	std::memcpy(&header, d, pOffset);

	// Don't accept packets with mismatching magic numbers
	if (ntohl(header.Magic) != MAGIC)
		throw ParseException("Magic numbers do not match");

	payload.assign(d + pOffset, d + len);

	if (ntohs(header.Version) != CURRENTVERSION)
		throw ParseException("Datagram format version mismatch");

	return true;
}

bool DGram::Parse(const std::vector<uint8_t> &d) {
	return Parse(d.data(), d.size());
}

void DGram::Payload(const std::vector<uint8_t> &p) {
	payload = p;
}

void DGram::Payload(const std::string &s) {
	payload = std::vector<uint8_t>(s.begin(), s.end());
}

std::vector<uint8_t> DGram::Payload() const {
	return payload;
}

std::vector<uint8_t> DGram::Raw() const {
	const size_t hSize = sizeof(DGramHeader_t);
	std::vector<uint8_t> ret;

	/* Reserve since we already know the size */
	ret.reserve(hSize + payload.size());

	/* Start from the header and build a vector of its raw data */
	uint8_t *begin = (uint8_t*)&header;
	ret = std::vector<uint8_t>(begin, begin + hSize);

	/* Append the payload to the constructed vector */
	ret.insert(std::end(ret), std::begin(payload), std::end(payload));

	return ret;
}

void DGram::Singleton(const std::vector<uint8_t> &p) {
	/* Singleton packets have NO_OFFSET and total size is only that of their
	 * contents */
	Header(NO_OFFSET, p.size());

	/* Configure payload as normal */
	Payload(p);
}

void DGram::Header(uint32_t offset, uint32_t contextlen) {
	/* Store as network-order */
	header.ContextLen = htonl(contextlen);
	header.DataOffset = htonl(offset);
	header.Magic = htonl(MAGIC);
	header.Version = htons(CURRENTVERSION);
}

DGramHeader_t DGram::Header() {
	return header;
}

uint32_t DGram::TxId() const {
	return txid;
}

void DGram::TxId(uint32_t id) {
	txid = id;
} }
