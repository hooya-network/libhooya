#pragma once

#include <map>
#include <vector>
#include <cstdint>

namespace hooya::crypto {
/**
 * Possible MutliHash types
 */
typedef enum multihash {
	KECCAK256_MHASH
} multihash_t;

/**
 * MultiHash -> unique identifier mapping
 * https://github.com/multiformats/multicodec/blob/master/table.csv
 */
static const std::map<multihash_t, std::vector<uint8_t>> MHASH_MAGIC = {
	{ KECCAK256_MHASH, std::vector<uint8_t>{ 0x1b, 0x20 } },
}; }
