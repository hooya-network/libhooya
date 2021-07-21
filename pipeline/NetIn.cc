#include "hooya/pipeline/NetIn.hh"

namespace hooya::pipeline {
NetIn::NetIn() :
teardown(false) {
}

NetIn::~NetIn() {
	if (!worker)
		return;

	/* Interrupt FIFO listener and wait for it to exit */
	teardown = true;
	fromFIFO->Interrupt(1);
	exited.Lower();

	/* Join FIFO listener so we do not leak the thread */
	worker->join();
}

void NetIn::SetFromFIFO(std::shared_ptr<DGramFIFO> f) {
	fromFIFO = f;
}

void NetIn::SetToFIFO(std::shared_ptr<NetDataFIFO> f) {
	toFIFO = f;
}

bool NetIn::SpawnWorker() {
	if (worker || !fromFIFO || !toFIFO)
		return false;

	/* thread to process FIFO and manage cache */
	worker = std::make_unique<std::thread>(std::thread([&]() {
		do try { accumulateAndForward();
		} catch (const exception::Interrupt &e) {
			break;
		} while (!teardown);

		/* On exit, signal to the dtor that we are ready to be joined */
		exited.Raise();
	}));

	return true;
}

void NetIn::accumulateAndForward() {
	/* Get incoming datagrams */
	sock::DGram in = fromFIFO->Pop();
	sock::DGramHeader_t header = in.Header();

	/* \todo Qualify incoming data
	 * + Does this data fit in an appropriate transmission window?
	 * + Should we shift the window?
	 *
	 * ... all should be broken out into separate methods
	 */
	size_t total = cache.Add(in.TxId(), header.DataOffset, in.Payload());

	if (total < header.ContextLen) {
		/* Not all of the payload has been received */
		return;
	}

	if (total > header.ContextLen) {
		/* TODO Raise an error flag because we were fed too much data */
	}

	/* Entire payload received, forward to next stage */
	toFIFO->Push(cache.Evict(in.TxId()));
} }
