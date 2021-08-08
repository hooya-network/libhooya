#include "hooya/crypto/Keccak.hh"

/* Implementation */
#include <botan/hex.h>
#include <botan/hash.h>

namespace hooya::crypto {
Keccak::Keccak() :
hashing(false)
{ }

void Keccak::Hash(const std::string &in) {
	Update(in);
	Finalize();
}

void Keccak::Update(const std::string &in) {
	if (not hashing) {
		/* Instantiate Botan hash function if not created yet */
		hashF = Botan::HashFunction::create(KECCAK_NAME);
		assert(hashF);
	}

	hashing = true;
	hashF->update(in);
}

void Keccak::Finalize() {
	hashing = false;

	/* Store bytes in array of constant size */
	hashF->final(hashed);
}

std::string Keccak::AsHex(bool uppercase) const {
	return Botan::hex_encode(hashed, KECCAK_SIZE, uppercase);
}

std::vector<uint8_t> Keccak::AsBytes() const {
	return std::vector<uint8_t>(hashed, hashed + KECCAK_SIZE);
}

uint32_t Keccak::AsKey() {
	return *(uint32_t*)hashed;
} }
