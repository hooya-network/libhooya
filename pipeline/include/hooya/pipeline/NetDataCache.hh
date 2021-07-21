#pragma once

#include <map>
#include <exception>
#include "hooya/sock/DGram.hh"
#include "hooya/net/NetData.hh"

namespace hooya::pipeline {
/**
 * Accumulator for incoming datagrams before being packaged into a datastream
 * and forwarded
 */
class NetDataCache {
public:
	/**
	 * Write more data at the given offset to the data at a key
	 * \param key Cache key
	 * \param offset Start writing / appending data at this offset
	 * \param data Actual data to append or write
	 * \return Size of data at key after append / write operation
	 */
	size_t Add(uint32_t key, size_t offset, const std::vector<uint8_t> &data);

	/**
	 * Remove value at a key from the cache
	 * \param key Cache key
	 */
	msg::NetData Evict(uint32_t key);
private:
	/**
	 * Cold hard cache
	 */
	std::map<uint32_t, hooya::msg::NetData> cache;
}; }
