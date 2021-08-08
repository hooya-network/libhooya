#include "CIDTest.hh"

TEST_F(CIDTest, CIDKeccak256) {
	hooya::crypto::Keccak k;

	k.Hash("Asakusa");
	auto cid = hooya::crypto::CID(k);

	cid.ChangeBase(hooya::crypto::BASE16_MBASE);
	ASSERT_EQ(cid.Out(), "1b200f7611ea58490d36e75e96f1254da0986cfe82e8265c89c755273578a12d185e4c");

	cid.ChangeBase(hooya::crypto::BASE16UPPER_MBASE);
	ASSERT_EQ(cid.Out(), "1B200F7611EA58490D36E75E96F1254DA0986CFE82E8265C89C755273578A12D185E4C");
}
