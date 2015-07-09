#include "XcbConnection.hpp"

#include <stdexcept>

namespace xcm
{
namespace xcb
{
XcbConnection::XcbConnection() : connection(xcb_connect(nullptr, nullptr))
{
	if(xcb_connection_has_error(connection))
		throw std::runtime_error("Connecting to DISPLAY failed.");
}

XcbConnection::~XcbConnection()
{
	xcb_disconnect(connection);
}

xcb_connection_t *XcbConnection::get() const
{
	return connection;
}

void XcbConnection::flush()
{
	if(xcb_flush(connection) <= 0)
		throw std::runtime_error("Flushing XCB connection failed.");
}
}
}
