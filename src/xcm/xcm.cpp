#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <system_error>
#include <thread>
#include <vector>
#include <experimental/optional>

#include <signal.h>

#include <xcb/xproto.h>

#include "xcmc/util/getErrnoError.hpp"
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

void handleTermination(int)
{
	EVENT_LOOP.interrupt();
}

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
		struct sigaction action;
		std::memset(&action, 0, sizeof(struct sigaction));
		action.sa_handler = handleTermination;
		int ret = sigaction(SIGINT, &action, nullptr);
		if(ret != 0)
			throw std::runtime_error(xcm::util::getErrnoError());
		ret = sigaction(SIGTERM, &action, nullptr);
		if(ret != 0)
			throw std::runtime_error(xcm::util::getErrnoError());

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
