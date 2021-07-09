#pragma once

#include <thread>
#include <future>
#include <algorithm>
#include <vector>
#include <memory>

namespace hooya::net {
class udpS;
class UDPManager {
public:
	/**
	 * Initialize before main DoUntilInterrupt() loop
	 */
	void Setup();

	/**
	 * Read data from all sockets in a loop until Interrupt()
	 */
	void DoUntilInterrupt();

	/**
	 * Close all sockets and return from DoUntilInterrupt()
	 */
	void Interrupt();

	/**
	 * Open a new channel for communications on the given ip, port
	 */
	bool AddChannel(std::string ip, unsigned port = 0);

	/**
	 * Open a port on the loopback address
	 */
	bool AddLoopbackChannel();
private:
	static const unsigned MAX_SOCKET = 64;
	static const unsigned LOOPBACK_PORT = 1720;

	std::vector<std::shared_ptr<udpS>> sS;
}; }
