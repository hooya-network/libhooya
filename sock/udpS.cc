#include "udpS.hh"

/* Implementation detail */
#include <net/route.h>
#include "hooya/sock/Exception.hh"
#include <iostream>

namespace hooya::sock {
udpS::udpS() :
bound(false)
{
	// Just default to a v4 socket
	v4LocalDefaults();
	v4RemoteDefaults();

	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
}

udpS::~udpS() {
	shutclose();
}

void udpS::SendOne(const DGram &egress) {
	if (!them.sin_port)
		throw BindException("No egress port configured");

	ssize_t sentLen = -1;
	auto packet = egress.Raw();

	if (them.sin_family == AF_INET) {
		sentLen = sendto(fd, packet.data(), packet.size(), 0,
			(struct sockaddr*)&them, sizeof(them));
	}

	if (sentLen < 0)
		throw BindException(errno);
}

const DGram udpS::GetOne() {
	ssize_t len;
	socklen_t myLen, iLen;
	struct sockaddr *myInfo, *iInfo;
	hooya::sock::DGram d;

	switch(me.sin_family) {
	case AF_INET:
		myInfo = (struct sockaddr *)&me;
		iInfo = (struct sockaddr *)&ingressaddr;
		myLen = sizeof(struct sockaddr_in);
		iLen = sizeof(struct sockaddr_in);
		break;
	case AF_INET6:
		myInfo = (struct sockaddr *)&me6;
		iInfo = (struct sockaddr *)&ingressaddr6;
		myLen = sizeof(struct sockaddr_in6);
		iLen = sizeof(struct sockaddr_in6);
		break;
	default:
		throw "No address family configured";
	}

	if (!bound) {
		if (bind(fd, myInfo, myLen))
			throw BindException(errno);

		bound = true;
	}

	do {
		len = recvfrom(fd, iBuf, MAX_SINGLE_PAYLOAD, 0, iInfo, &iLen);

		if (len < 0) {
			/* Timed out */
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				throw Timeout("Socket receive timed out");

			/* Other errors */
			throw(BindException(errno));
		}

		try {
			d.Parse(iBuf, len);
			break;
		} catch (const ParseException &e) {
			std::cout << e.what() << std::endl;
		}
	} while (true);

	// TODO Set them.sin_port if them.sin_port == 0 for "auto" detection of
	// egress port

	return d;
}

void udpS::local(in_addr_t inet, uint16_t port) {
	/* No change - do not disturb file descriptor */
	if (inet == ntohl(me.sin_addr.s_addr))
		return;

	/* Close old FD */
	shutclose();

	/* Delete existing local socket config */
	clearLocal();

	/* Change the local address used for listening */
	me.sin_family = AF_INET;
	me.sin_addr.s_addr = inet;
	me.sin_port = port;

	/* Re-open the file descriptor with new connection details */
	fd = socket(AF_INET, SOCK_DGRAM, 0);
}

void udpS::remote(in_addr_t inet, uint16_t port) {
	/* Delete existing remote client config */
	clearRemote();

	/* Change the local address used for sending */
	them.sin_family = AF_INET;
	them.sin_addr.s_addr = inet;
	them.sin_port = port;
}

bool udpS::RecvTimeout(unsigned sec, unsigned usec) {
	timeout.tv_sec = sec;
	timeout.tv_usec = usec;
	return !setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
}

void udpS::Local(const std::string &rHost, uint16_t port) {
	in_addr_t inetA;

	if ((inetA = inet_addr(rHost.c_str()))) {
		local(inetA, htons(port));
		return;
	}
	throw BindException("Unable to parse remote address");
}

void udpS::Remote(const std::string &rHost, uint16_t port) {
	in_addr_t inetA;

	if ((inetA = inet_addr(rHost.c_str()))) {
		remote(inetA, htons(port));
		return;
	}
	throw BindException("Unable to parse remote address");
}

void udpS::LocalAnyV4() {
	local(INADDR_ANY);
}

/* void udpS::LocalAnyV6() {
	Local(in6addr_any);
} */

void udpS::shutclose() {
	if (bound) {
		shutdown(fd, SHUT_RDWR);
		bound = false;
	}
	close(fd);
}

void udpS::v4LocalDefaults() {
	clearLocal();

	me.sin_family = AF_INET;
	me.sin_port = 0;
	me.sin_addr.s_addr = INADDR_ANY;

	fd = socket(AF_INET, SOCK_DGRAM, 0);

	/* Restore timeout */
	/* if (timeout.tv_sec || timeout.tv_usec)
		setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)); */
}

void udpS::v6LocalDefaults() {
	clearLocal();

	me6.sin6_family = AF_INET6;
	me6.sin6_port = 0;
	me6.sin6_addr = IN6ADDR_ANY_INIT;

	fd = socket(AF_INET6, SOCK_DGRAM, 0);

	/* Restore timeout */
	/* if (timeout.tv_sec || timeout.tv_usec)
		setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)); */
}

void udpS::v4RemoteDefaults() {
	clearRemote();

	them.sin_family = AF_INET;
}

void udpS::v6RemoteDefaults() {
	clearRemote();

	them6.sin6_family = AF_INET6;
}

void udpS::clearLocal() {
	std::memset(&me, 0, sizeof(me));
	std::memset(&me6, 0, sizeof(me));
}

void udpS::clearRemote() {
	std::memset(&them, 0, sizeof(them));
	std::memset(&them6, 0, sizeof(them));
} }
