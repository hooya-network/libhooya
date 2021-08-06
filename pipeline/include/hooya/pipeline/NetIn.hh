#pragma once

#include <thread>
#include "hooya/pipeline/CountingSemaphore.hh"
#include "hooya/pipeline/NetDataCache.hh"
#include "hooya/pipeline/FIFO.hh"
#include "hooya/sock/DGram.hh"
#include "hooya/net/NetData.hh"
#include "hooya/pipeline/Exception.hh"
#include "hooya/crypto/Keccak.hh"

namespace hooya::pipeline {
using DGramFIFO = FIFO<hooya::sock::DGram>;
using NetDataFIFO = FIFO<hooya::msg::NetData>;

/**
 * Combines the payloads from incoming datagrams intelligently into a full
 * message (NetData). It reads many DGram from a FIFO and will write NetData to
 * a different FIFO.
 */
class NetIn {
public:
	NetIn();
	~NetIn();

	/**
	 * Set incoming datagram FIFO
	 * \param f FIFO to read from
	 */
	void SetFromFIFO(std::shared_ptr<DGramFIFO> f);

	/*
	 * Set FIFO where outgoing data streams are fed
	 * \param f FIFO to write to
	 */
	void SetToFIFO(std::shared_ptr<NetDataFIFO> f);

	/**
	 * Sort info from incoming FIFO, append to existing data and forward
	 * completed NetData
	 */
	bool SpawnWorker();
private:

	/* Worker thread */
	std::unique_ptr<std::thread> worker;

	/* Input */
	std::shared_ptr<DGramFIFO> fromFIFO;

	/* Output */
	std::shared_ptr<NetDataFIFO> toFIFO;

	/* A place to temporarily accumulate payloads from incoming datagrams */
	NetDataCache cache;

	/* Signal that the worker thread should be joined in dtor */
	bool teardown;

	/* Main loop */
	void accumulateAndForward();

	/* Signal to dtor that thread is ready to be joined */
	CountingSemaphore exited;
}; }
