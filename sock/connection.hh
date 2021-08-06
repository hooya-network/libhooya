#pragma once

#include <string>
#include <arpa/inet.h>
#include "hooya/sock/Exception.hh"

namespace hooya::sock {
class Connection {
public:
	/**
	 * Derive address info from AF_INET sockaddr
	 * \param sa Address details
	 */
	void Derive(const struct sockaddr_in &sa);

	/**
	 * Derive address info from AF_INET6 sockaddr
	 * \param sa Address details
	 */
	void Derive(const struct sockaddr_in6 &sa6);

	/**
	 * Derive address info from sockaddr
	 * \param sa Address details
	 */
	void Derive(const struct sockaddr *sa);

	/**
	 * User-readable address in IP:Port format
	 * \return IP:Port
	 */
	std::string Address() const;
private:

	/**
	 * Derived remote address
	 */
	std::string address;

	/**
	 * Derived port (host-order)
	 */
	unsigned long port;
}; }
