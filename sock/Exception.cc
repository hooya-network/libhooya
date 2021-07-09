#include "hooya/sock/Exception.hh"

/* Implementation detail */
#include <sys/socket.h>

namespace hooya::sock {
BindException::BindException(const char *desc) :
d{desc}
{ }

BindException::BindException(int eno) {
	switch(eno) {
		case EACCES:
			d = "The address is protected";
			break;
		case EADDRINUSE:
			d = "The given address is already in use";
			break;
		case EINVAL:
			d = "The socket is already bound";
			break;
		case ENOTSOCK:
			d = "The file descriptor does not refer to a socket";
			break;
		default:
			d = "Unknown bind error";
	}
}

ParseException::ParseException(const char *desc) :
d{desc}
{ } }
