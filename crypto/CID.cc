#include "hooya/crypto/CID.hh"

/* Implementation */
#include "bin2.hh"

namespace hooya::crypto {
CID::CID(const Keccak &k, multibase_t b) :
currHash(KECCAK256_MHASH),
currBase(b),
raw(k.AsBytes())
{ }

void CID::ChangeBase(multibase_t b) {
	currBase = b;
}

std::vector<uint8_t> CID::mhashMagic() {
	return MHASH_MAGIC.at(currHash);
}

std::vector<uint8_t> CID::mbaseMagic() {
	return MBASE_MAGIC.at(currBase);
}

std::string CID::Out() {
	auto bMagic = mbaseMagic();
	auto hMagic = mhashMagic();

	/* MultiHash + MultiBase + raw data */
	auto full = hMagic;
	full.insert(full.end(), bMagic.begin(), bMagic.end());
	full.insert(full.end(), raw.begin(), raw.end());

	/* Actually encode raw data */
	switch(currBase) {
	case BASE16_MBASE:
		return Bin2::Base16(full);
	case BASE16UPPER_MBASE:
		return Bin2::Base16(full, true);
	default:
		assert(false);
	}
} }
