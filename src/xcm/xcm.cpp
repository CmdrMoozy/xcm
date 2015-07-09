#include <cstdlib>

#include "xcmc/xcb/XcbConnection.hpp"
#include "xcmc/xcb/XcbWindow.hpp"

int main(int, char const **)
{
	xcm::xcb::XcbConnection connection;
	xcm::xcb::XcbWindow window(connection);

	return EXIT_SUCCESS;
}
