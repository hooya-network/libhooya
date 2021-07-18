#pragma once

#include <list>
#include <thread>
#include "hooya/sock/Remote.hh"
#include "hooya/pipeline/FIFO.hh"
#include "hooya/sock/Exception.hh"

namespace hooya::pipeline {
/**
 * Provide a single, threaded interface to the network
 */
class Uplink {
public:
	Uplink();
	~Uplink();

	/**
	 * Resources represented by Uplink are non-copyable
	 */
	Uplink(Uplink &other) = delete;

	/**
	 * Spin up a new thread for listening and responding to local queries on the
	 * loopback address
	 */
	void SpawnLoopback();

	/**
	 * Sets the FIFO to which datagrams should be forwarded
	 * \param f Forward FIFO
	 */
	void ConnectForwardFIFO(std::shared_ptr<DGramFIFO> f);

private:

	/**
	 * Forward a datagram to the FIFO
	 */
	void forward(const hooya::sock::DGram &d);

	/**
	 * Forwarding FIFO
	 */
	std::shared_ptr<DGramFIFO> fifo;

	/**
	 * Persistent loopback listener
	 */
	std::shared_ptr<hooya::sock::Remote> persistent;

	/**
	 * Connections to remote clients
	 */
	std::list<std::shared_ptr<hooya::sock::Remote>> remotes;

	/**
	 * All threads under the Uplink's management
	 */
	std::list<std::thread> threads;
}; }
