set(xcmc_SOURCES

	xcb/XcbConnection.cpp
	xcb/XcbConnection.hpp

)

add_library(xcmc STATIC ${xcmc_SOURCES})
