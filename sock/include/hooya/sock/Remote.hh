#pragma once

#include <sys/socket.h>
#include "hooya/sock/DGram.hh"

namespace hooya::sock {
class udpS;
/**
 * Represents a direct connection with a client on the network
 */
class Remote {
public:
	Remote();
	~Remote();

	/**
	 * Making a duplicate of a remote makes no sense, resource is unique
	 */
	Remote(Remote& other) = delete;

	/**
	 * Get a single datagram from the connected UDP socket
	 * \return Next available datagram on the ingress side
	 */
	DGram Get();

	/**
	 * Forward a single datagram to the connected UDP socket
	 * \param d Datagram to egress
	 * \return Success
	 */
	void Send(const DGram &egress);

	/**
	 * Socket is bound to any port / address
	 * \return True if this socket is bound and ready to receive data
	 */
	bool IsBound();

	/**
	 * Send or receive only from given host on any address
	 * \param rHost Associated remote host (v4-only for now)
	 * \param port Associated remote port
	 */
	void Associate(const std::string &rHost, int port = 0);

	/**
	 * Declare this as the loopback remote
	 */
	void LoopbackIngress();

	/**
	 * Declare this as the loopback remote
	 */
	void LoopbackEgress();

	/**
	 * Set a send / receive timeout
	 * \param sec Seconds
	 * \param usec Microseconds
	 * \return Success setting timeout
	 */
	bool Timeout(int sec, int usec = 0);

	/**
	 * Stop listening on the socket and throw ShutdownException from Get()
	 */
	void Shutdown();

	/**
	 * Default port for loopback communication with (e.g.) hooya-gtk
	 */
	static const uint16_t DEFAULT_LOOPBACK_PORT = 8251;
private:
	/**
	 * Socket to ingress / egress on
	 */
	udpS *sock;
}; }
