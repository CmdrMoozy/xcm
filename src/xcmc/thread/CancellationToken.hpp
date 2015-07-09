#ifndef xcmc_thread_CancellationToken_HPP
#define xcmc_thread_CancellationToken_HPP

#include <memory>
#include <experimental/optional>

namespace xcm
{
namespace thread
{
class CancellationTokenHandle;

class CancellationToken
{
public:
	CancellationToken();

	CancellationToken(CancellationToken const &) = default;
	~CancellationToken() = default;

	CancellationToken &operator=(CancellationToken const &) = default;

private:
	friend class CancellationTokenHandle;

	CancellationToken(std::shared_ptr<void> t);

	std::shared_ptr<void> token;
};

class CancellationTokenHandle
{
public:
	explicit CancellationTokenHandle(CancellationToken const &token);

	CancellationTokenHandle(CancellationTokenHandle const &) = default;
	~CancellationTokenHandle() = default;

	CancellationTokenHandle &
	operator=(CancellationTokenHandle const &) = default;

	std::experimental::optional<CancellationToken> lock() const;

private:
	std::weak_ptr<void> handle;
};
}
}

#endif
