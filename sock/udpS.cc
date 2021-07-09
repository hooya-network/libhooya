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
	v4Defaults();
}

udpS::~udpS() {
	shutclose();
}

const hooya::sock::DGram udpS::GetOne() {
	unsigned len;
	socklen_t sLen, cLen;
	struct sockaddr *sInfo, *cInfo;
	hooya::sock::DGram d;

	switch(family()) {
	case AF_INET:
		sInfo = (struct sockaddr *)&servaddr;
		cInfo = (struct sockaddr *)&clientaddr;
		sLen = sizeof(struct sockaddr_in);
		cLen = sizeof(struct sockaddr_in);
		break;
	case AF_INET6:
		sInfo = (struct sockaddr *)&servaddr6;
		cInfo = (struct sockaddr *)&clientaddr6;
		sLen = sizeof(struct sockaddr_in6);
		cLen = sizeof(struct sockaddr_in6);
		break;
	default:
		throw "No address family configured";
	}

	if (!bound) {
		if (bind(fd, sInfo, sLen))
			throw BindException(errno);

		bound = true;
	}

	do {
		len = recvfrom(fd, iBuf, MAX_SINGLE_PAYLOAD, 0, cInfo, &cLen);
		if (errno)
			throw(BindException(errno));
		try {
			d.Parse(iBuf, len);
			break;
		} catch (const ParseException &e) {
			std::cout << e.what() << std::endl;
		}
	} while (true);

	return d;
}

void udpS::AnyV4() {
	Address(INADDR_ANY);
}

void udpS::AnyV6() {
	Address(in6addr_any);
}

void udpS::Address(in_addr_t inet) {
	// Close old FD
	shutclose();

	// Reset conninfo to defaults
	v4Defaults();

	// But change the bind address
	servaddr.sin_addr.s_addr = inet;
}

void udpS::Address(in6_addr inet6) {
	// Close old FD
	shutclose();

	// Reset conninfo to defaults
	v6Defaults();

	// But change the bind address
	servaddr6.sin6_addr = inet6;
}

bool udpS::Address(const std::string &a) {
	in_addr_t inetA = inet_addr(a.c_str());
	if (inetA)
		Address(inetA);
	// V6 implementation goes here ^-^
	else
		return false;

	return true;
}

void udpS::shutclose() {
	if (bound) {
		shutdown(fd, SHUT_RDWR);
		bound = false;
	}
	close(fd);
}

bool udpS::Port(unsigned p) {
	if (p > 65535)
		return false;

	if (family() == AF_INET) {
		servaddr.sin_port = htons(p);
		return true;
	}

	servaddr6.sin6_port = htons(p);
	return true;
}

sa_family_t udpS::family() const {
	return servaddr.sin_family ? AF_INET : AF_INET6;
}

void udpS::v4Defaults() {
	std::memset(&servaddr, 0, sizeof(servaddr));
	std::memset(&servaddr6, 0, sizeof(servaddr6));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = 0;
	servaddr.sin_addr.s_addr = INADDR_ANY;

	fd = socket(AF_INET, SOCK_DGRAM, 0);
}

void udpS::v6Defaults() {
	std::memset(&servaddr, 0, sizeof(servaddr));
	std::memset(&servaddr6, 0, sizeof(servaddr6));

	servaddr6.sin6_family = AF_INET6;
	servaddr6.sin6_port = 0;
	servaddr6.sin6_addr = IN6ADDR_ANY_INIT;

	fd = socket(AF_INET6, SOCK_DGRAM, 0);
} }
