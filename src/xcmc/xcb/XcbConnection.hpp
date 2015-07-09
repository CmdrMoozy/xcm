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
	~XcbConnection();

	xcb_connection_t *get() const;

private:
	xcb_connection_t *connection;
};
}
}

#endif
