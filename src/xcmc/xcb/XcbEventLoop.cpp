#include "XcbEventLoop.hpp"

#include <cstdlib>
#include <memory>
#include <stdexcept>

#include <sys/select.h>
#include <sys/time.h>

#include <xcb/xcb.h>
#include <xcb/xproto.h>

#include "xcmc/util/getErrnoError.hpp"

namespace
{
xcb_generic_event_t *waitForEvent(xcm::xcb::XcbConnection const &connection,
                                  std::atomic<bool> &interrupt)
{
	int fd = xcb_get_file_descriptor(connection.get());
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(fd, &fds);

	// Poll every 0.25s.
	const struct timespec TIMEOUT = {0l, 250000000l};

	xcb_generic_event_t *event = nullptr;
	while(interrupt.load(std::memory_order_seq_cst))
	{
		int ret = pselect(fd + 1, &fds, nullptr, nullptr, &TIMEOUT,
		                  nullptr);
		if(ret < 0)
			throw std::runtime_error(xcm::util::getErrnoError());

		if(ret > 0)
		{
			if((event = xcb_poll_for_event(connection.get())) !=
			   nullptr)
			{
				break;
			}
		}
	}
	return event;
}
}

namespace xcm
{
namespace xcb
{
XcbConnection &XcbEventLoop::get()
{
	return connection;
}

XcbConnection const &XcbEventLoop::get() const
{
	return connection;
}

void XcbEventLoop::interrupt()
{
	running.store(false, std::memory_order_seq_cst);
}

void XcbEventLoop::reset()
{
	running.store(true, std::memory_order_seq_cst);
}

void XcbEventLoop::run()
{
	xcb_generic_event_t *e;
	while((e = waitForEvent(connection, running)) != nullptr)
	{
		std::unique_ptr<xcb_generic_event_t, void (*)(void *)> event(
		        e, std::free);
	}
}
}
}
