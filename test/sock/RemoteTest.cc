#include "RemoteTest.hh"

static const std::vector<uint8_t> SMALL_DATA = {
	/* ☕️ */
	0xC0, 0xFF, 0xEE
};

/**
 * Try to send a packet without configuring egress port
 */
TEST_F(RemoteTest, SendNoPort) {
	hooya::sock::Remote r;
	hooya::sock::DGram dgram;

	/* Only sending a singleton packet */
	dgram.Singleton(SMALL_DATA);

	/* Attempt to send from localhost on any port into the void */
	r.Associate("127.0.0.1");

	ASSERT_THROW(r.Send(dgram), hooya::sock::BindException);
}

/**
 * Send a packet into the void
 */
TEST_F(RemoteTest, SendSingletonLoopback) {
	hooya::sock::Remote r;
	hooya::sock::DGram dgram;

	/* Only send a singleton packet */
	dgram.Singleton(SMALL_DATA);

	r.LoopbackEgress();

	/* Dump eet */
	r.Send(dgram);
}

/**
 * Send a packet and receive the same one
 */
TEST_F(RemoteTest, SendReceiveSingletonLoopback) {
	hooya::sock::Remote r1, r2;
	hooya::sock::DGram egress, ingress;

	/* Construct datagram from some simple data */
	egress.Singleton(SMALL_DATA);

	/* Loopback producer / consumer pair */
	r1.LoopbackIngress();
	r2.LoopbackEgress();

	/* Quick timeout for port ingress */
	ASSERT_EQ(r1.Timeout(0, 10000), true);

	/* Egress the packet */
	std::thread([&]() {
		r2.Send(egress);
	}).detach();

	/* Ingress the same packet; try multiple times so we can quickly bail out if
	 * the packet never showed up to avoid infinite loop over recvfrom() */
	for (int i = 10; i; --i) {
		try {
			ingress = r1.Get();
		} catch (hooya::sock::Timeout &e) {
			/* ◦°˚\(*❛‿❛)/˚°◦ */
		}
	}

	/* Received datagram should exactly match the sent datagram */
	ASSERT_EQ(ingress.Raw(), egress.Raw());
}
