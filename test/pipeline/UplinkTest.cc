#include "UplinkTest.hh"

static const std::vector<uint8_t> SMALL_DATA = {
	/* ☕️ */
	0xC0, 0xFF, 0xEE
};

TEST_F(UplinkTest, ForwardOne) {
	hooya::pipeline::Uplink uplink;
	hooya::sock::Remote r1;
	hooya::sock::DGram egress;

	auto fifo = std::make_shared<hooya::pipeline::DGramFIFO>();

	/* Smol singleton */
	egress.Singleton(SMALL_DATA);

	/* FIFO to receive messages */
	uplink.ConnectForwardFIFO(fifo);

	/* New listener on loopback address, standard port */
	uplink.SpawnLoopback();

	/* Send datagram on loopback */
	r1.LoopbackEgress();
	r1.Send(egress);

	/* Pop datagram off FIFO and compare to the one we sent to loopback */
	ASSERT_EQ(fifo->Pop().Raw(), egress.Raw());
}
