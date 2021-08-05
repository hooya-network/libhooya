#pragma once

#include <memory>
#include <vector>
#include <string>
#include <cstdint>
#include <assert.h>

namespace Botan {
	class HashFunction;
}

namespace hooya::crypto {
class Keccak {
public:
	Keccak();

	/**
	 * Update an existing hash or start a new one
	 */
	void Update(const std::string &in);

	/**
	 * Finalize hash and calculate over all data passed through Update
	 */
	void Finalize();

	/**
	 * Display finalized hash data as hex digits
	 * \param uppercase Set for uppercase hex digits
	 * \return Hex digits representing hashed data
	 */
	std::string AsHex(bool uppercase = false);
private:
	/**
	 * We're using Keccak-256
	 */
	const char *KECCAK_NAME = "Keccak-1600(256)";

	/**
	 * Keccak-256 result in size_t bytes
	 */
	static const size_t KECCAK_SIZE = 256 / 8;

	/**
	 * Keep track of when we're hashing and when we're fnalized
	 */
	bool hashing;

	/**
	 * Hashed result
	 */
	uint8_t hashed[KECCAK_SIZE];

	/**
	 * Botan hash function for calculating the Keccak-256
	 */
	std::shared_ptr<Botan::HashFunction> hashF;
}; }
