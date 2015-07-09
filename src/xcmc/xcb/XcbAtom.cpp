#include "XcbAtom.hpp"

#include <cstdlib>
#include <stdexcept>

#include <xcb/xproto.h>

#include "xcmc/xcb/XcbConnection.hpp"

namespace
{
xcb_intern_atom_cookie_t requestAtom(xcm::xcb::XcbConnection const &connection,
                                     xcm::xcb::XcbAtomType type,
                                     bool create = false)
{
	std::string typeString{xcm::xcb::xcbAtomTypeToString(type)};
	return xcb_intern_atom(connection.get(), create ? 0 : 1,
	                       typeString.length(), typeString.data());
}
}

namespace xcm
{
namespace xcb
{
std::string xcbAtomTypeToString(XcbAtomType type)
{
	switch(type)
	{
	case XcbAtomType::CLIPBOARD_MANAGER:
		return "CLIPBOARD_MANAGER";
	default:
		throw std::runtime_error("Unrecognized XcbAtomType.");
	}
}

XcbAtom::XcbAtom(XcbConnection const &connection,
                 xcb_intern_atom_cookie_t cookie)
        : atom()
{
	xcb_intern_atom_reply_t *a =
	        xcb_intern_atom_reply(connection.get(), cookie, nullptr);
	if(a == nullptr)
		throw std::runtime_error("Getting XcbAtom response failed.");
	atom = *a;
	std::free(a);
}

XcbAtom::XcbAtom(XcbConnection const &connection, XcbAtomType type, bool create)
        : XcbAtom(connection, requestAtom(connection, type, create))
{
}

xcb_atom_t XcbAtom::get() const
{
	return atom.atom;
}

std::vector<XcbAtom> constructXcbAtoms(XcbConnection const &connection,
                                       std::vector<XcbAtomType> const &types,
                                       bool create)
{
	std::vector<xcb_intern_atom_cookie_t> cookies;
	for(auto const &type : types)
		cookies.push_back(requestAtom(connection, type, create));

	std::vector<XcbAtom> atoms;
	for(auto const &cookie : cookies)
		atoms.emplace_back(connection, cookie);
	return atoms;
}
}
}
