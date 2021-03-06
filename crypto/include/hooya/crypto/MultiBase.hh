#pragma once

#include <map>
#include <vector>
#include <string>
#include <cstdint>

namespace hooya::crypto {
/**
 * Possible MultiBase types
 */
typedef enum multibase {
	BASE8_MBASE,
	BASE16_MBASE,
	BASE16UPPER_MBASE,
	BASE64_MBASE,
} multibase_t;

/**
 * MultiBase -> unique identifier mapping
 * https://github.com/multiformats/multibase/blob/master/multibase.csv
 */
static const std::map<multibase_t, std::vector<uint8_t>> MBASE_MAGIC = {
	{ BASE8_MBASE, std::vector<uint8_t>{ 7 } },
	{ BASE16_MBASE, std::vector<uint8_t>{ 15 } },
	{ BASE16UPPER_MBASE, std::vector<uint8_t>{ 15 } },
	{ BASE64_MBASE, std::vector<uint8_t>{ 63 } },
}; }
