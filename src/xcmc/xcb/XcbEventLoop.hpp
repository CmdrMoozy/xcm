#ifndef xcmc_xcb_XcbEventLoop_HPP
#define xcmc_xcb_XcbEventLoop_HPP

#include <atomic>
#include <utility>
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
	          connection(std::forward<Arg>(arg)...)
	{
	}

	XcbEventLoop(XcbEventLoop const &) = delete;
	~XcbEventLoop() = default;

	XcbEventLoop &operator=(XcbEventLoop const &) = delete;

	XcbConnection &get();
	XcbConnection const &get() const;

	void interrupt();
	void reset();
	void run();

private:
	std::experimental::optional<thread::CancellationToken> token;
	XcbConnection connection;
};
}
}

#endif
