set(xcmc_SOURCES

	util/getErrnoError.cpp
	util/getErrnoError.hpp

	xcb/XcbAtom.cpp
	xcb/XcbAtom.hpp
	xcb/XcbConnection.cpp
	xcb/XcbConnection.hpp
	xcb/XcbWindow.cpp
	xcb/XcbWindow.hpp

)

add_library(xcmc STATIC ${xcmc_SOURCES})
