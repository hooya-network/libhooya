#include "UplinkTest.hh"

TEST_F(UplinkTest, ForwardOne) {
	hooya::pipeline::Uplink uplink;
	hooya::sock::Remote r1;
	hooya::sock::DGram egress;

	auto fifo = std::make_shared<hooya::pipeline::DGramFIFO>();

	/* Smol singleton */
	egress.Singleton(SIMPLE_PACKET);

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
