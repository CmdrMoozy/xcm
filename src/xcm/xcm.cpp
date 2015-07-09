#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <xcb/xproto.h>

#include "xcmc/xcb/XcbAtom.hpp"
#include "xcmc/xcb/XcbConnection.hpp"
#include "xcmc/xcb/XcbWindow.hpp"

int main(int, char const **)
{
	try
	{
		xcm::xcb::XcbConnection connection;
		xcm::xcb::XcbWindow window(connection);

		std::vector<xcm::xcb::XcbAtom> atoms =
		        xcm::xcb::constructXcbAtoms(
		                connection,
		                {xcm::xcb::XcbAtomType::CLIPBOARD_MANAGER});

		connection.flush();

		return EXIT_SUCCESS;
	}
	catch(std::exception const &e)
	{
		std::cerr << "ERROR: " << e.what() << "\n";
	}
	catch(...)
	{
		std::cerr << "ERROR: Caught unknown exception.\n";
	}

	return EXIT_FAILURE;
}
