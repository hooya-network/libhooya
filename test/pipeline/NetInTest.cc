#include "NetInTest.hh"

using DGramFIFO = hooya::pipeline::FIFO<hooya::sock::DGram>;
using NetDataFIFO = hooya::pipeline::FIFO<hooya::msg::NetData>;

TEST_F(NetInTest, Singleton) {
	hooya::pipeline::NetIn dut;

	/* Singleton packet */
	hooya::sock::DGram dgram;
	dgram.Parse(SIMPLE_PACKET);

	/* Input datagrams to NetIn execution unit */
	auto in = std::make_shared<DGramFIFO>();

	/* Output data stream from NetIn execution unit */
	auto out = std::make_shared<NetDataFIFO>();

	/* Wire everything together and create the worker thread */
	dut.SetFromFIFO(in);
	dut.SetToFIFO(out);
	dut.SpawnWorker();

	/* Provide an input singleton packet and check the output data stream */
	in->Push(dgram);
	auto res = out->Pop();
	ASSERT_EQ(res.Raw(), SIMPLE_PACKET_PAYLOAD);

	/* KECCAK256("IP:Port" + "0") as uint32_t
	 *
	 * This will identify the reply-to address when we ultimately exit the
	 * pipeline for egress (not in this test) */
	ASSERT_EQ(res.Stamp(), 2991736836);
}
