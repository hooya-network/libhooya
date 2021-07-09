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
	 * Get a single datagram from the connected UDP socket
	 * \return Next available datagram on the ingress side
	 */
	DGram Get();

	/**
	 * Forward a single datagram to the connected UDP socket
	 * \param d Datagram to egress
	 * \return Success
	 */
	bool Send(const DGram &d);

	/**
	 * Socket is able to ingress / egress datagrams
	 * \return True if this socket can forward and / or receive datagrams
	 */
	bool IsValid();

	/**
	 * Listen on all address on the given port
	 * \param port Remote to port associate with
	 */
	bool Associate(int port = 0);
private:
	/**
	 * Socket to ingress / egress on
	 */
	udpS *sock;


}; }
