#pragma once

#include <vector>
#include <cassert>
#include "hooya/crypto/Keccak.hh"
#include "MultiHash.hh"
#include "MultiBase.hh"

namespace hooya::crypto {
/**
 * Content ID - MultiHash encoded with MultiBase
 */
class CID {
public:
	/**
	 * Keccak-256 initialization
	 * \param k Keccak hash containing the data to represent
	 * \param b Optional MultiBase
	 */
	CID(const Keccak &k, multibase_t b = DEFAULT_MBASE);

	/**
	 * Change MultiBase representation
	 * \param b MultiBase
	 */
	void ChangeBase(multibase_t b);

	/**
	 * Represent hash binary data with given MultiHash and MultiBase
	 * \return MultiHash + MultiBase + encoded binary data
	 */
	std::string Out();

private:
	/**
	 * Shortcut to fetch MultiHash identifier
	 * \return Byte representation of MultiHash identifier
	 */
	std::vector<uint8_t> mhashMagic();

	/**
	 * Shortcut to fetch MultiBase identifier
	 * \return Byte representation of MultiBase identifier
	 */
	std::vector<uint8_t> mbaseMagic();

	/**
	 * Configured MultiHash
	 */
	multihash_t currHash;

	/**
	 * Configured MultiBase
	 */
	multibase_t currBase;

	/**
	 * Default MultiBase
	 */
	static const multibase_t DEFAULT_MBASE = BASE16_MBASE;

	/**
	 * Raw data from the hash function
	 */
	std::vector<uint8_t> raw;
}; }
