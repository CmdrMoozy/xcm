#include "XcbEventLoop.hpp"

#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <memory>
#include <stdexcept>

#include <sys/select.h>
#include <sys/time.h>

#include <xcb/xcb.h>
#include <xcb/xproto.h>

#include "xcmc/util/getErrnoError.hpp"

namespace
{
xcb_generic_event_t *pollForEvents(std::mutex &workMutex,
                                   std::deque<std::function<void()>> &work,
                                   std::deque<std::function<void()>> &requests,
                                   xcm::xcb::XcbConnection const &connection,
                                   xcm::thread::CancellationTokenHandle handle)
{
	int fd = xcb_get_file_descriptor(connection.get());
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(fd, &fds);

	// Poll every 0.25s.
	const struct timespec TIMEOUT = {0l, 250000000l};

	xcb_generic_event_t *event = nullptr;
	for(;;)
	{
		std::experimental::optional<xcm::thread::CancellationToken>
		        token = handle.lock();
		if(!token) break;

		// Deal with any non-event work first.

		std::deque<std::function<void()>> todo;

		{
			std::lock_guard<std::mutex> lock(workMutex);
			std::swap(work, todo);
		}
		while(!todo.empty())
		{
			(todo.front())();
			todo.pop_front();
		}

		{
			std::lock_guard<std::mutex> lock(workMutex);
			std::swap(requests, todo);
		}
		while(!todo.empty())
		{
			(todo.front())();
			todo.pop_front();
		}

		// Handle any X events.

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

void XcbEventLoop::post(std::function<void()> const &f)
{
	std::lock_guard<std::mutex> lock(workMutex);
	work.push_back(f);
}

void XcbEventLoop::post(std::vector<std::function<void()>> const &fs)
{
	std::lock_guard<std::mutex> lock(workMutex);
	std::copy(fs.begin(), fs.end(), std::back_inserter(work));
}

void XcbEventLoop::interrupt()
{
	token = std::experimental::nullopt;
}

void XcbEventLoop::reset()
{
	if(!token) token.emplace();
}

void XcbEventLoop::run()
{
	xcb_generic_event_t *e;
	thread::CancellationTokenHandle handle(*token);
	while((e = pollForEvents(workMutex, work, requests, connection,
	                         handle)) != nullptr)
	{
		std::unique_ptr<xcb_generic_event_t, void (*)(void *)> event(
		        e, std::free);
	}
}
}
}
