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

	/* MultiBase + MultiHash + raw data */
	auto full = bMagic;
	full.insert(full.end(), hMagic.begin(), hMagic.end());
	full.insert(full.end(), raw.begin(), raw.end());

	/* Actually encode raw data */
	switch(currBase) {
	case BASE8_MBASE:
		return Bin2::Base8(full);
	case BASE16_MBASE:
		return Bin2::Base16(full);
	case BASE16UPPER_MBASE:
		return Bin2::Base16(full, true);
	case BASE64_MBASE:
		return Bin2::Base64(full);
	default:
		assert(false);
	}
} }
