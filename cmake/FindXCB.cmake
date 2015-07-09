# Try to find XCB. Once done this will define:
#
#  LIBXCB_FOUND
#  LIBXCB_LIBRARIES
#  LIBXCB_INCLUDE_DIR
#  LIBXCB_DEFINITIONS

find_package(PkgConfig)
pkg_check_modules(PKG_XCB xcb)

set(LIBXCB_DEFINITIONS ${PKG_XCB_CFLAGS})

find_path(LIBXCB_INCLUDE_DIR xcb/xcb.h ${PKG_XCB_INCLUDE_DIRS})
find_library(LIBXCB_LIBRARIES NAMES xcb libxcb PATHS ${PKG_XCB_LIBRARY_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(XCB DEFAULT_MSG LIBXCB_LIBRARIES LIBXCB_INCLUDE_DIR)

mark_as_advanced(LIBXCB_INCLUDE_DIR LIBXCB_LIBRARIES XCBPROC_EXECUTABLE)
