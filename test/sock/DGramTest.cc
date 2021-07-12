#include "DGramTest.hh"

// Network-order
static const std::vector<uint8_t> SIMPLE_PACKET = {
	0x68, 0x59, 0x61, 0x21, // Magic number
	0x00, 0x00,             // Version number
	0x00, 0x00,             // Reserved
	0x00, 0x00, 0x00, 0x03, // Context length
	0x00, 0x00, 0x00, 0x00, // This offset
	0xC0, 0xFF, 0xEE        // Data
};

// Correct packet but mismatched version number
static const std::vector<uint8_t> SIMPLE_PACKET_VERSION_MISMATCH = {
	0x68, 0x59, 0x61, 0x21, // Magic number
	0x00, 0xAA,             // *Future* version number
	0x00, 0x00,             // Reserved
	0x00, 0x00, 0x00, 0x03, // Context length
	0x00, 0x00, 0x00, 0x00, // This offset
	0xC0, 0xFF, 0xEE        // Data
};

// Packet too smol
static const std::vector<uint8_t> SIMPLE_PACKET_TOO_SMOL = {
	0x68, 0x59, 0x61, 0x21, // Magic number
	0x00, 0x00,             // Version number
	0x00, 0x00,             // Reserved
	0x00, 0x00, 0x00, 0x03, // Context length
	0x00, 0x00, 0x00, 0x00  // This offset
	// Too short - packets with no payload are not valid
};

// Line noise / not a packet
static const std::vector<uint8_t> LINE_NOISE = {
	7, 176, 34, 95, 237, 57, 4, 12, 109, 166, 233, 171, 195, 54, 84, 172, 94,
	18, 52, 18, 57, 160, 43, 229, 187, 177, 125, 89, 60, 26, 250, 127, 134, 66,
	133, 152, 244, 97, 58, 212, 186, 136, 146, 51, 146, 210, 201, 6, 199, 236
};

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
