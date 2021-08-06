#pragma once

#include <stdexcept>
#include <vector>
#include <cstdint>

namespace hooya::msg {
/**
 * One single message (request, query, response etc.) carried across the
 * network. This is the combination of all bytes in a series of payloads carried
 * by some number of UDP packets
 */
class NetData {
public:
	/**
	 * Insert or overwrite data at this position
	 * \param at Starting position of first byte
	 * \param inData Incoming data
	 * \return New size in bytes of all data
	 */
	size_t AddAt(size_t at, const std::vector<uint8_t> &inData);

	/**
	 * Size in bytes of underlying data
	 * \return Size
	 */
	size_t Size() const;

	/**
	 * Raw payload bytes in network-order
	 */
	const std::vector<uint8_t> &Raw();

	void Stamp(uint32_t s);
	uint32_t Stamp();

private:
	std::vector<uint8_t> data;

	uint32_t stamp;
}; }
