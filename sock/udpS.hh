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
	 * Sets the address and port to listen on and send from
	 *
	 * \param lAddr Local address
	 * \param port Port
	 */
	void Local(const std::string &lAddr, uint16_t port = 0);

	/**
	 * Sets where datagrams should be sent; this will also begin filtering
	 * incoming packets so only valid packets from this remote host are
	 * acted on
	 * \param rHost Remote host
	 * \param port Port
	 *
	 * \warn port = 0 will cause egress to fail unless overridden. Only useful
	 * for the loopback socket with a well-known port number
	 */
	void Remote(const std::string &rHost, uint16_t port = 0);

	/**
	 * Don't be particular about the address or port from which to send
	 * datagrams, just let the OS figure it out.
	 */
	void LocalAnyV4();

	/**
	 * Specify a timeout to use when waiting on ingress data. Useful for tests
	 * and probably some other things. Generally this will throw a
	 * hooya::sock::Timeout if the socket is inactive for the given time.
	 * \param sec Seconds to wait
	 * \param usec Additional microseconds to wait
	 */
	bool RecvTimeout(unsigned sec, unsigned usec = 0);

	/**
	 * Read one single message from the underlying socket
	 * \return Ingress datagram
	 */
	const DGram GetOne();

	/**
	 * Send one single message to the underlying socket
	 * \param egress Egress datagram
	 */
	void SendOne(const DGram &egress);

	void Shutdown();

	bool IsBound();

private:
	/**
	 * Largest possible size for any single inbound or outbound datagram
	 */
	static const size_t MAX_SINGLE_PAYLOAD = 4096;

	/**
	 * V4 address structures
	 */
	struct sockaddr_in me, them, ingressaddr;

	/**
	 * V6 address structures
	 */
	struct sockaddr_in6 me6, them6, ingressaddr6;

	/**
	 * Like Remote() but network-order so it's internal
	 */
	void remote(in_addr_t inet, uint16_t port);

	/**
	 * Like Local() but network-order so it's internal
	 */
	void local(in_addr_t inet, uint16_t port = 0);

	/**
	 * Shutdown a socket and / or close an FD
	 */
	void shutclose();

	/**
	 * Bind socket according to fd and address family
	 */
	void bindSock();

	/**
	 * Reset ingress info to v4 defaults
	 */
	void v4LocalDefaults();

	/**
	 * Reset ingress info to v6 defaults
	 */
	void v6LocalDefaults();

	/**
	 * Reset egress info to v4 defaults
	 */
	void v4RemoteDefaults();

	/**
	 * Reset egress info to v6 defaults
	 */
	void v6RemoteDefaults();

	/**
	 * Wipe any configuration done by local()
	 */
	void clearLocal();

	/**
	 * Wipe any configuration done by remote()
	 */
	void clearRemote();

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
	bool shuttingDown;
	unsigned fd;

	/**
	 * Socket receive timeout
	 * \todo Make timeout also work for send()
	 * \todo Define a ClearTimeout() function to reset this value and clear any
	 * configured timeouts
	 */
	struct timeval timeout;
}; }
