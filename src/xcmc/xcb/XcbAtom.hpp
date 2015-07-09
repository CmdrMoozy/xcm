#ifndef xcmc_xcb_XcbAtom_HPP
#define xcmc_xcb_XcbAtom_HPP

#include <string>
#include <vector>

#include <xcb/xcb.h>

namespace xcm
{
namespace xcb
{
class XcbConnection;

enum class XcbAtomType
{
	CLIPBOARD_MANAGER
};

std::string xcbAtomTypeToString(XcbAtomType type);

class XcbAtom
{
public:
	XcbAtom(XcbConnection const &connection,
	        xcb_intern_atom_cookie_t cookie);
	XcbAtom(XcbConnection const &connection, XcbAtomType type,
	        bool create = false);

	XcbAtom(XcbAtom const &) = delete;
	XcbAtom(XcbAtom &&) = default;

	~XcbAtom() = default;

	XcbAtom &operator=(XcbAtom const &) = delete;
	XcbAtom &operator=(XcbAtom &&) = default;

	xcb_atom_t get() const;

private:
	xcb_intern_atom_reply_t atom;
};

/*!
 * Construct a series of XcbAtom objects of the given types. This is more
 * efficient than constructing the atoms one at a time, since we issue all of
 * the requests first before blocking to wait for the responses (this should
 * mean less waiting, and is a better way to use libxcb).
 *
 * The resulting objects should be std::move'd out of the returned vector.
 *
 * \param connection The XCB connection to use.
 * \param types The types of atoms to construct.
 * \param create Whether or not atoms should be created if they do not exist.
 * \return The newly constructed atoms.
 */
std::vector<XcbAtom> constructXcbAtoms(XcbConnection const &connection,
                                       std::vector<XcbAtomType> const &types,
                                       bool create = false);
}
}

#endif
