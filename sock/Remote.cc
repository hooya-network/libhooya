#include "hooya/sock/Remote.hh"

/* Implementation detail */
#include "udpS.hh"
#include "hooya/sock/Exception.hh"
#include <iostream>

namespace hooya::sock {
Remote::Remote() :
sock(new udpS)
{ }

Remote::~Remote() {
	delete sock;
}

bool Remote::Associate(int port) {
	sock->AnyV4();
	return sock->Port(port);
}

DGram Remote::Get() {
	do {
		try {
			return sock->GetOne();
		} catch (const BindException &e) {
			std::cout << e.what() << std::endl;
		}
	} while (true);
} }
