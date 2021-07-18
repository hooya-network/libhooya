#include "NetDataTest.hh"

static const std::vector<uint8_t> one = {
	1, 2, 3, 4
}, two = {
	5, 6, 7, 8
};

TEST_F(NetDataTest, OnlyAppend) {
	hooya::msg::NetData d;

	d.AddAt(0, one);

	ASSERT_EQ(d.Size(), 4);
}

TEST_F(NetDataTest, AppendTwice) {
	hooya::msg::NetData d;

	d.AddAt(d.AddAt(0, one), two);

	ASSERT_EQ(d.Size(), 8);
}

TEST_F(NetDataTest, Overwrite) {
	hooya::msg::NetData d;

	d.AddAt(0, one);
	d.AddAt(0, two);

	ASSERT_EQ(d.Size(), 4);
}

TEST_F(NetDataTest, OverwriteOffset) {
	hooya::msg::NetData d;

	d.AddAt(0, one);
	d.AddAt(2, two);

	ASSERT_EQ(d.Size(), 6);
}

TEST_F(NetDataTest, SparseAppend) {
	hooya::msg::NetData d;

	d.AddAt(0, one);
	d.AddAt(17, two);

	ASSERT_EQ(d.Size(), 20);
}
