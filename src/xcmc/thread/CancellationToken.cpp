#include "CancellationToken.hpp"

namespace xcm
{
namespace thread
{
CancellationToken::CancellationToken()
        : token(this, [](void *)
                {
	        })
{
}

CancellationToken::CancellationToken(std::shared_ptr<void> t) : token(t)
{
}

CancellationTokenHandle::CancellationTokenHandle(CancellationToken const &token)
        : handle(token.token)
{
}

std::experimental::optional<CancellationToken>
CancellationTokenHandle::lock() const
{
	std::shared_ptr<void> token = handle.lock();
	if(!token) return std::experimental::nullopt;
	return CancellationToken(token);
}
}
}
