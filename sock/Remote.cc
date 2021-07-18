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

void Remote::Shutdown() {
	sock->Shutdown();
}

void Remote::Associate(const std::string &rHost, int port) {
	/* Let the OS choose the specific port & address to send from */
	sock->LocalAnyV4();

	/* Datagrams destined for this remote host & port combination */
	sock->Remote(rHost, port);
}

void Remote::LoopbackIngress() {
	/* Listen on the default port */
	sock->Local("127.0.0.1", DEFAULT_LOOPBACK_PORT);

	/* Only talk with localhost (but don't set up an egress port) */
	sock->Remote("127.0.0.1");
}

void Remote::LoopbackEgress() {
	/* Outbound traffic originates from localhost */
	sock->Local("127.0.0.1");

	/* Only talk with the instance on the default port, also on localhost */
	sock->Remote("127.0.0.1", DEFAULT_LOOPBACK_PORT);
}

void Remote::Send(const DGram &d) {
	sock->SendOne(d);
}

bool Remote::IsBound() {
	return sock->IsBound();
}

bool Remote::Timeout(int sec, int usec) {
	return sock->RecvTimeout(sec, usec);
}

DGram Remote::Get() {
	return sock->GetOne();
} }
