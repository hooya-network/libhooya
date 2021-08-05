#include "KeccakTest.hh"

TEST_F(KeccakTest, HashEmpty) {
	hooya::crypto::Keccak k;
	k.Update("");
	k.Finalize();
	ASSERT_EQ(k.AsHex(), "c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470");
}

TEST_F(KeccakTest, HashSimple) {
	hooya::crypto::Keccak k;
	k.Update("Urahara");
	k.Finalize();
	ASSERT_EQ(k.AsHex(), "f2683eb454cc5da40d304368d0114f39bfbd36f1d2d03a29329622a95d19b1f3");
}

TEST_F(KeccakTest, HashUpdateTwice) {
	hooya::crypto::Keccak k;
	k.Update("Ura");
	k.Update("hara");
	k.Finalize();
	ASSERT_EQ(k.AsHex(), "f2683eb454cc5da40d304368d0114f39bfbd36f1d2d03a29329622a95d19b1f3");
}
