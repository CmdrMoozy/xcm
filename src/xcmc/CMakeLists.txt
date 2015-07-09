set(xcmc_SOURCES

	xcb/XcbConnection.cpp
	xcb/XcbConnection.hpp
	xcb/XcbWindow.cpp
	xcb/XcbWindow.hpp

)

add_library(xcmc STATIC ${xcmc_SOURCES})
