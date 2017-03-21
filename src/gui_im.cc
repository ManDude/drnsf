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
#include "../imgui/imgui.h"
#include "gui.hh"

namespace gui {
namespace im {

void main_menu_bar(
	const std::function<void()> &f)
{
	if (BeginMainMenuBar()) {
		f();
		EndMainMenuBar();
	}
}

void menu(
	const std::string &text,
	const std::function<void()> &f)
{
	if (!f) {
		BeginMenu(text.c_str(),false);
		return;
	}
	if (BeginMenu(text.c_str())) {
		f();
		EndMenu();
	}
}

void menu_item(
	const std::string &text,
	const std::function<void()> &f)
{
	if (!f) {
		MenuItem(text.c_str(),nullptr,false,false);
		return;
	}
	if (MenuItem(text.c_str())) {
		f();
	}
}

void menu_separator()
{
	Separator();
}

}
}