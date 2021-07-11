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

	// Interpret network-order longs and shorts
	header.Magic = ntohl(header.Magic);
	header.Version = ntohs(header.Version);
	header.Reserved = ntohs(header.Reserved);

	// Don't accept packets with mismatching magic numbers
	if (header.Magic != MAGIC) {
		throw ParseException("Magic numbers do not match");
	}

	payload.assign(d + pOffset, d + len);

	if (header.Version != CURRENTVERSION)
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

std::vector<uint8_t> DGram::Payload() {
	return payload;
}

void DGram::Header(int offset, int contextlen) {
	header.ContextLen = contextlen;
	header.DataOffset = offset;
	header.Magic = MAGIC;
} }
