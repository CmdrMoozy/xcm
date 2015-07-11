#ifndef xcmc_xcb_XcbEventLoop_HPP
#define xcmc_xcb_XcbEventLoop_HPP

#include <cstdlib>
#include <deque>
#include <functional>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>
#include <experimental/optional>

#include <xcb/xcb.h>

#include "xcmc/thread/CancellationToken.hpp"
#include "xcmc/xcb/XcbConnection.hpp"

namespace xcm
{
namespace xcb
{
class XcbEventLoop
{
public:
	template <typename... Arg> XcbEventLoop(Arg... arg);

	XcbEventLoop(XcbEventLoop const &) = delete;
	~XcbEventLoop() = default;

	XcbEventLoop &operator=(XcbEventLoop const &) = delete;

	XcbConnection &get();
	XcbConnection const &get() const;

	void post(std::function<void()> const &f);
	void post(std::vector<std::function<void()>> const &fs);

	template <typename Cookie, typename Reply, typename... Arg>
	void request(Cookie (*requestFn)(xcb_connection_t *, Arg...),
	             Reply *(*replyFn)(xcb_connection_t *, Cookie,
	                               xcb_generic_error_t **),
	             std::function<void(Reply const &)> const &responseHandler,
	             Arg... arg);

	void interrupt();
	void reset();
	void run();

private:
	std::experimental::optional<thread::CancellationToken> token;
	std::mutex workMutex;
	std::deque<std::function<void()>> work;
	std::deque<std::function<void()>> requests;
	XcbConnection connection;
};

template <typename... Arg>
XcbEventLoop::XcbEventLoop(Arg... arg)
        : token(thread::CancellationToken()),
          workMutex(),
          work(),
          connection(std::forward<Arg>(arg)...)
{
}

template <typename Cookie, typename Reply, typename... Arg>
void XcbEventLoop::request(
        Cookie (*requestFn)(xcb_connection_t *, Arg...),
        Reply *(*replyFn)(xcb_connection_t *, Cookie, xcb_generic_error_t **),
        std::function<void(Reply const &)> const &responseHandler, Arg... arg)
{
	Cookie cookie = requestFn(connection.get(), std::forward<Arg>(arg)...);
	std::lock_guard<std::mutex> lock(workMutex);
	requests.emplace_back(
	        [this, cookie, replyFn, responseHandler]()
	        {
		        Reply *r = replyFn(connection.get(), cookie, nullptr);
		        std::unique_ptr<Reply, void (*)(void *)> reply(
		                r, std::free);
		        responseHandler(*reply);
		});
}
}
}

#endif
