#include "hooya/sock/DGram.hh"

#include <cstring>
#include "hooya/sock/Exception.hh"

namespace hooya::sock {
bool DGram::Parse(const char *d, size_t len) {
	const size_t headerSize = sizeof(header);
	const std::byte *const headerStart = (std::byte*)d;
	const std::byte *const payloadEnd = (std::byte*)d + len;
	const std::byte *headerBoundary = headerStart + headerSize;

	// Header cannot be larger than the datagram itself lol
	if (headerSize >= len)
		throw ParseException("Datagram too small to parse");

	// Extract header and payload raw data
	std::memcpy(&header, d, headerSize);
	payload = std::vector<std::byte>(headerBoundary, payloadEnd);

	return true;
} }
