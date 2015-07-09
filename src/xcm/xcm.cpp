#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <xcb/xproto.h>

#include "xcmc/xcb/XcbAtom.hpp"
#include "xcmc/xcb/XcbEventLoop.hpp"
#include "xcmc/xcb/XcbWindow.hpp"

int main(int, char const **)
{
	try
	{
		xcm::xcb::XcbEventLoop eventLoop;
		xcm::xcb::XcbWindow window(eventLoop.get());

		std::vector<xcm::xcb::XcbAtom> atoms =
		        xcm::xcb::constructXcbAtoms(
		                eventLoop.get(),
		                {xcm::xcb::XcbAtomType::CLIPBOARD_MANAGER});

		eventLoop.get().flush();

		eventLoop.run();

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
