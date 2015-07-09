#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <system_error>
#include <thread>
#include <vector>
#include <experimental/optional>

#include <xcb/xproto.h>

#include "xcmc/xcb/XcbAtom.hpp"
#include "xcmc/xcb/XcbEventLoop.hpp"
#include "xcmc/xcb/XcbWindow.hpp"

namespace
{
static xcm::xcb::XcbEventLoop EVENT_LOOP;
static std::experimental::optional<xcm::xcb::XcbWindow> WINDOW{
        std::experimental::nullopt};
static std::experimental::optional<std::vector<xcm::xcb::XcbAtom>> ATOMS{
        std::experimental::nullopt};

void joinThread(std::thread &thread)
{
	try
	{
		thread.join();
	}
	catch(std::system_error const &e)
	{
		if(e.code() != std::errc::invalid_argument) throw;
	}
}
}

int main(int, char const **)
{
	try
	{
		std::thread thread([]()
		                   {
			                   EVENT_LOOP.run();
			           });

		EVENT_LOOP.post(
		        []()
		        {
			        WINDOW.emplace(EVENT_LOOP.get());
			        ATOMS.emplace(xcm::xcb::constructXcbAtoms(
			                EVENT_LOOP.get(),
			                {xcm::xcb::XcbAtomType::
			                         CLIPBOARD_MANAGER}));
			        EVENT_LOOP.get().flush();
			});

		joinThread(thread);
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
