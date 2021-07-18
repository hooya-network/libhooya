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
	size_t Size();
private:
	std::vector<uint8_t> data;
}; }
