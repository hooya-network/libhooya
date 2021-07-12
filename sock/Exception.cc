#include "hooya/sock/Exception.hh"

/* Implementation detail */
#include <sys/socket.h>
#include <cstring>

namespace hooya::sock {
BindException::BindException(const char *desc) :
d{desc}
{ }

BindException::BindException(int eno) {
	d = std::strerror(eno);
}

ParseException::ParseException(const char *desc) :
d{desc}
{ } }
