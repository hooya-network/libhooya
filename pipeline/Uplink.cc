#include "hooya/pipeline/Uplink.hh"

namespace hooya::pipeline {
Uplink::Uplink() {
	persistent = std::make_shared<hooya::sock::Remote>();
	persistent->LoopbackIngress();
	remotes.push_back(persistent);
}

void Uplink::ConnectForwardFIFO(std::shared_ptr<DGramFIFO> f) {
	fifo = f;
}

Uplink::~Uplink() {
	/* Close all out remotes */
	for (auto &r : remotes)
		r->Shutdown();

	/* Join remote threads */
	for (auto &t : threads)
		t.join();
}

void Uplink::SpawnLoopback() {
	threads.push_back(std::thread([&]() {
		while (true) {
			try {
				/* Forward all ingress packets towards FIFO for processing */
				forward(persistent->Get());
			} catch (const hooya::sock::ShutdownException &e) {
				/* Someone called Shutdown(), stop forwarding */
				break;
			} catch (const hooya::sock::BindException &e) {
				/* Already bound? Bad port? Something for sure */
				std::cout << e.what() << std::endl;
				break;
			}
		}
	}));
}

void Uplink::forward(const hooya::sock::DGram d) {
	/* FIFO receives ingress datagrams */
	fifo->Push(d);
} }
