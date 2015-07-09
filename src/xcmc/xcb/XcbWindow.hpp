#ifndef xcmc_xcb_XcbWindow_HPP
#define xcmc_xcb_XcbWindow_HPP

#include <cstdint>

#include <xcb/xcb.h>

namespace xcm
{
namespace xcb
{
class XcbConnection;

class XcbWindow
{
public:
	XcbWindow(XcbConnection const &connection);
	XcbWindow(XcbWindow const &) = delete;
	~XcbWindow() = default;
	XcbWindow &operator=(XcbWindow const &) = delete;

private:
	uint32_t xid;
};
}
}

#endif
