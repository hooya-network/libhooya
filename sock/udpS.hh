#pragma once

#include <mutex>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include "hooya/sock/DGram.hh"

namespace hooya::sock {
class udpS {
public:
	/**
	 * Construct a UDP Socket
	 */
	udpS();

	/**
	 * Close and shutdown open socket and / or FD
	 */
	~udpS();

	/**
	 * Bind to given address
	 * \param a IP address string
	 */
	bool Address(const std::string &a);

	/**
	 * Bind to given v4 address
	 * \param inet AF_INET address
	 */
	void Address(in_addr_t inet);

	/**
	 * Bind to given v6 address
	 * \param inet6 AF_INET6 address
	 */
	void Address(in6_addr inet6);

	/**
	 * Bind to wildcard v4 address
	 */
	void AnyV4();

	/**
	 * Bind to wildcard v6 address
	 */
	void AnyV6();

	// TODO
	// bool Address(in_addr_t6 inet);

	/**
	 * Sets the port to bind the socket to
	 * \param p Port number
	 */
	bool Port(unsigned p);

	/**
	 * Read one single message from the underlying socket
	 * \return Message as network-order bytes
	 */
	const hooya::sock::DGram GetOne();

private:
	/**
	 * Largest possible size for any single inbound or outbound datagram
	 */
	static const size_t MAX_SINGLE_PAYLOAD = 4096;

	struct sockaddr_in servaddr, clientaddr;
	struct sockaddr_in6 servaddr6, clientaddr6;

	/**
	 * Shutdown a socket and / or close an FD
	 */
	void shutclose();

	/**
	 * Reset to v4 defaults
	 */
	void v4Defaults();

	/**
	 * Reset to v6 defaults
	 */
	void v6Defaults();

	/**
	 * Configured address family of the socket
	 * This is changed only by calls to Address(std::string)
	 */
	sa_family_t family() const;

	/**
	 * Incoming data buffer
	 */
	uint8_t iBuf[MAX_SINGLE_PAYLOAD];

	std::mutex buf_l;

	bool bound;
	unsigned fd;
}; }
