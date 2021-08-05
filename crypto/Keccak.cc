#include "hooya/crypto/Keccak.hh"

/* Implementation */
#include <botan/hex.h>
#include <botan/hash.h>

namespace hooya::crypto {
Keccak::Keccak() :
hashing(false)
{ }

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

std::string Keccak::AsHex(bool uppercase) {
	if (hashing)
		Finalize();

	return Botan::hex_encode(hashed, KECCAK_SIZE, uppercase);
} }
