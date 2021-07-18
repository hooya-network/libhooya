#include "DGramTest.hh"

/**
 * Parse SIMPLE_PACKET and ensure payload was parsed correctly
 */
TEST_F(DGramTest, ParseSimple0xC0FFEE) {
	hooya::sock::DGram d;

	EXPECT_NO_THROW(d.Parse(SIMPLE_PACKET));
	auto p = d.Payload();

	ASSERT_EQ(d.Payload()[0], 0xC0);
	ASSERT_EQ(d.Payload()[1], 0xFF);
	ASSERT_EQ(d.Payload()[2], 0xEE);
}

/**
 * Parse a packet with a mismatched version number
 */
TEST_F(DGramTest, ParseMismatchedVersion) {
	hooya::sock::DGram d;

	EXPECT_THROW(
		d.Parse(SIMPLE_PACKET_VERSION_MISMATCH),
		hooya::sock::ParseException
	);
}

/**
 * Attempt (and expect to fail) to parse line noise
 */
TEST_F(DGramTest, ParseLineNoise) {
	hooya::sock::DGram d;

	EXPECT_THROW(d.Parse(LINE_NOISE),
		hooya::sock::ParseException
	);
}

/**
 * Attempt (and expect to fail) to parse a packet with no payload
 */
TEST_F(DGramTest, ParseSmallPacket) {
	hooya::sock::DGram d;

	EXPECT_THROW(d.Parse(SIMPLE_PACKET_TOO_SMOL),
		hooya::sock::ParseException
	);
}
