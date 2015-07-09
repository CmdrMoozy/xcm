#ifndef xcmc_xcb_XcbEventLoop_HPP
#define xcmc_xcb_XcbEventLoop_HPP

#include <atomic>
#include <deque>
#include <functional>
#include <mutex>
#include <utility>
#include <vector>
#include <experimental/optional>

#include "xcmc/thread/CancellationToken.hpp"
#include "xcmc/xcb/XcbConnection.hpp"

namespace xcm
{
namespace xcb
{
class XcbEventLoop
{
public:
	template <typename... Arg>
	XcbEventLoop(Arg... arg)
	        : token(thread::CancellationToken()),
	          workMutex(),
	          work(),
	          connection(std::forward<Arg>(arg)...)
	{
	}

	XcbEventLoop(XcbEventLoop const &) = delete;
	~XcbEventLoop() = default;

	XcbEventLoop &operator=(XcbEventLoop const &) = delete;

	XcbConnection &get();
	XcbConnection const &get() const;

	void post(std::function<void()> const &f);
	void post(std::vector<std::function<void()>> const &fs);

	void interrupt();
	void reset();
	void run();

private:
	std::experimental::optional<thread::CancellationToken> token;
	std::mutex workMutex;
	std::deque<std::function<void()>> work;
	XcbConnection connection;
};
}
}

#endif
