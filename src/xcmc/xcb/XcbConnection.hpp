#ifndef xcmc_xcb_XcbConnection_HPP
#define xcmc_xcb_XcbConnection_HPP

#include <xcb/xcb.h>

namespace xcm
{
namespace xcb
{
class XcbConnection
{
public:
	XcbConnection();
	XcbConnection(XcbConnection const &) = delete;
	~XcbConnection();
	XcbConnection &operator=(XcbConnection const &) = delete;

	xcb_connection_t *get() const;

private:
	xcb_connection_t *connection;
};
}
}

#endif
