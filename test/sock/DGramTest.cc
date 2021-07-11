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

