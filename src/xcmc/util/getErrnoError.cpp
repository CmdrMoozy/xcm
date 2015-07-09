#include "getErrnoError.hpp"

#include <cerrno>
#include <cstring>

namespace xcm
{
namespace util
{
std::string getErrnoError()
{
	std::string errorString = "Unknown error.";
	char buf[1024];
	return std::string(strerror_r(errno, buf, 1024));
}
}
}
