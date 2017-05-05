//
// DRNSF - An unofficial Crash Bandicoot level editor
// Copyright (C) 2017  DRNSF contributors
//
// See the AUTHORS.md file for more details.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "common.hh"
#include "edit.hh"

namespace drnsf {
namespace edit {

// (inner class) impl
// Implementation class for asset_infoview (PIMPL).
class asset_infoview::impl : private util::nocopy {
	friend class asset_infoview;

private:
	// (var) m_outer
	// A reference to the outer asset_infoview.
	asset_infoview &m_outer;

	// (var) m_proj
	// A reference to the project this tree applies to.
	res::project &m_proj;

	// (var) m_dummy
	// A dummy label to be replaced once this view is implemented.
	gui::label m_dummy;

	// (handler) h_asset_appear
	// Hooks the project's on_asset_appear event so that, if the selected
	// asset comes into existence, the details can be updated.
	decltype(res::project::on_asset_appear)::watch h_asset_appear;

	// (handler) h_asset_disappear
	// Hooks the project's on_asset_disappear event so that the currently
	// selected asset's details can be erased if it disappears.
	decltype(res::project::on_asset_disappear)::watch h_asset_disappear;

public:
	// (explicit ctor)
	// Initializes the widget and installs event handlers.
	explicit impl(
		asset_infoview &outer,
		gui::container &parent,
		res::project &proj) :
		m_outer(outer),
		m_proj(proj),
		m_dummy(parent,"[asset details here]")
	{
		h_asset_appear <<= [this](res::asset &asset) {
			// TODO
		};
		h_asset_appear.bind(m_proj.on_asset_appear);

		h_asset_disappear <<= [this](res::asset &asset) {
			// TODO
		};
		h_asset_disappear.bind(m_proj.on_asset_disappear);
	}
};

// declared in edit.hh
asset_infoview::asset_infoview(gui::container &parent,res::project &proj)
{
	M = new impl(*this,parent,proj);
}

// declared in edit.hh
asset_infoview::~asset_infoview()
{
	delete M;
}

// declared in edit.hh
void asset_infoview::show()
{
	M->m_dummy.show();
}

}
}