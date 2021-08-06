#include "connection.hh"

namespace hooya::sock {
void Connection::Derive(const struct sockaddr_in &sa) {
	char buf[INET_ADDRSTRLEN];

	port = ntohs(sa.sin_port);
	if (!inet_ntop(AF_INET, &sa, buf, INET_ADDRSTRLEN))
		throw ParseException("Unparsable IPv4 address");

	address = buf;
}

void Connection::Derive(const struct sockaddr_in6 &sa6) {
	char buf[INET6_ADDRSTRLEN];

	port = ntohs(sa6.sin6_port);
	if (!inet_ntop(AF_INET6, &sa6, buf, INET6_ADDRSTRLEN))
		throw ParseException("Unparsable IPv6 address");

	address = buf;
}

void Connection::Derive(const struct sockaddr *sa) {
	switch (sa->sa_family) {
	case AF_INET:
		Derive(*(sockaddr_in*)sa);
		return;
	case AF_INET6:
		Derive(*(sockaddr_in6*)sa);
		return;
	default:
		throw ParseException("Unknown address family");
	}
}

std::string Connection::Address() const {
	return address + ":" + std::to_string(port);
} }
