set(xcmc_SOURCES

	thread/CancellationToken.cpp
	thread/CancellationToken.hpp

	util/getErrnoError.cpp
	util/getErrnoError.hpp

	xcb/XcbAtom.cpp
	xcb/XcbAtom.hpp
	xcb/XcbConnection.cpp
	xcb/XcbConnection.hpp
	xcb/XcbEventLoop.cpp
	xcb/XcbEventLoop.hpp
	xcb/XcbWindow.cpp
	xcb/XcbWindow.hpp

)

add_library(xcmc STATIC ${xcmc_SOURCES})
