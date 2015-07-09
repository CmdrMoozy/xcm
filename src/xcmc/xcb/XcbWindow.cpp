#include "XcbWindow.hpp"

#include <xcb/xproto.h>

#include "xcmc/xcb/XcbConnection.hpp"

namespace xcm
{
namespace xcb
{
XcbWindow::XcbWindow(XcbConnection const &connection)
        : xid(xcb_generate_id(connection.get()))
{
	xcb_screen_t *screen =
	        xcb_setup_roots_iterator(xcb_get_setup(connection.get())).data;

	const uint32_t VALUE_LIST[] = {screen->black_pixel, 1,
	                               XCB_EVENT_MASK_PROPERTY_CHANGE};
	xcb_create_window(connection.get(), screen->root_depth, xid,
	                  screen->root, 0, 0, 1, 1, 0, XCB_COPY_FROM_PARENT,
	                  screen->root_visual,
	                  XCB_CW_BACK_PIXEL | XCB_CW_OVERRIDE_REDIRECT |
	                          XCB_CW_EVENT_MASK,
	                  &VALUE_LIST[0]);
	xcb_map_window(connection.get(), xid);
}
}
}
