#include "FIFOTest.hh"

/**
 * Build a FIFO and push some integers into it
 */
TEST_F(FIFOTest, Push) {
	hooya::pipeline::FIFO<int> f;

	f.Push(1);
	f.Push(2);
	f.Push(100);
	f.Push(500);

	ASSERT_EQ(f.Length(), 4);
}

TEST_F(FIFOTest, PushPopOrdered) {
	hooya::pipeline::FIFO<int> f;

	f.Push(100);
	f.Push(200);
	f.Push(300);
	ASSERT_EQ(f.Pop(), 100);
	ASSERT_EQ(f.Pop(), 200);
	ASSERT_EQ(f.Pop(), 300);
}

TEST_F(FIFOTest, ThreadedPushPop) {
	auto a = std::make_shared<hooya::pipeline::FIFO<int>>();
	auto b = std::make_shared<hooya::pipeline::FIFO<int>>();

	std::thread([a, b]() {
		ASSERT_EQ(b->Pop(), 1);
		a->Push(1);
	}).detach();

	b->Push(1);
	ASSERT_EQ(a->Pop(), 1);
}

TEST_F(FIFOTest, ThreadedManyPushPop) {
	const int limit = FIFO_STRESS_THREAD;
	auto a = std::make_shared<hooya::pipeline::FIFO<int>>();
	auto b = std::make_shared<hooya::pipeline::FIFO<int>>();

	std::thread([a, b]() {
		for (int i = 0; i < limit; ++i)
			a->Pop();
		for (int i = 0; i < limit; ++i)
			b->Push(i);
	}).detach();

	for (int i = 0; i < limit; ++i)
		a->Push(i);
	for (int i = 0; i < limit; ++i)
		b->Pop();
}

TEST_F(FIFOTest, ThreadMeHarder) {
	const int limit = FIFO_STRESS_THREAD;
	auto a = std::make_shared<hooya::pipeline::FIFO<int>>();
	auto b = std::make_shared<hooya::pipeline::FIFO<int>>();

	for (int i = 0; i < limit; ++i) {
		std::thread([a, b]() {
			a->Pop();
			b->Push(100);
		}).detach();

		std::thread([a, b]() {
			a->Push(100);
			b->Pop();
		}).detach();
	}
}
