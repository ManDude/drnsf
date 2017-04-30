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

#include "module.hh"
#include <stdio.h>
#include "gfx.hh"
#include "nsf.hh"
#include "misc.hh"

namespace drnsf {

BEGIN_MODULE

static std::vector<unsigned char> read_file(const std::string &filename)
{
	std::vector<unsigned char> data;
	FILE *f = fopen(filename.c_str(),"rb");
	if (!f)
		throw 5;
	fseek(f,0,SEEK_END);
	data.resize(ftell(f));
	fseek(f,0,SEEK_SET);
	fread(data.data(),data.size(),1,f);
	fclose(f);
	return data;
}

void frame(int delta)
{
	auto &&nx = m_core.m_proj.get_transact();
	auto &&ns = m_core.m_proj.get_asset_root();
	auto &&proj = m_core.m_proj;

	static std::vector<gfx::vertex> testbox_vertices = {
		{ -1, -1, -1 },
		{ +1, -1, -1 },
		{ +1, +1, -1 },
		{ -1, +1, -1 },
		{ +1, -1, +1 },
		{ -1, -1, +1 },
		{ -1, +1, +1 },
		{ +1, +1, +1 }
	};

	static std::vector<gfx::quad> testbox_polys = {
		{{ { 0, 0 }, { 1, 1 }, { 3, 3 }, { 2, 2 } }},
		{{ { 4, 4 }, { 5, 5 }, { 7, 7 }, { 6, 6 } }},
		{{ { 5, 5 }, { 0, 0 }, { 6, 6 }, { 3, 3 } }},
		{{ { 1, 1 }, { 4, 4 }, { 2, 2 }, { 7, 7 } }},
		{{ { 3, 3 }, { 2, 2 }, { 6, 6 }, { 7, 7 } }},
		{{ { 5, 5 }, { 4, 4 }, { 0, 0 }, { 1, 1 } }}
	};

	static std::vector<gfx::color> testbox_colors = {
		{ 255,   0,   0 },
		{   0, 255,   0 },
		{   0,   0, 255 },
		{ 255, 255,   0 },
		{   0, 255, 255 },
		{ 255,   0, 255 },
		{   0,   0,   0 },
		{ 255, 255, 255 }
	};

	if (ImGui::Button("Create test box")) {
		nx << [&](TRANSACT) {
			gfx::frame::ref frame = ns / "testbox/anim/0000";
			gfx::anim::ref anim = ns / "testbox/anim";
			gfx::mesh::ref mesh = ns / "testbox/mesh";
			gfx::model::ref model = ns / "testbox";

			TS.describe("Create test box");
			frame.create(TS,proj);
			frame->set_vertices(TS,testbox_vertices);

			anim.create(TS,proj);
			anim->set_frames(TS,std::vector<gfx::frame::ref>{frame});

			mesh.create(TS,proj);
			mesh->set_quads(TS,testbox_polys);
			mesh->set_colors(TS,testbox_colors);

			model.create(TS,proj);
			model->set_anim(TS,anim);
			model->set_mesh(TS,mesh);
		};
	}

	if (ImGui::Button("Load test scene")) {
		nx << [&](TRANSACT) {
			TS.describe("Load test scenery file");
			nsf::raw_entry::ref raw = ns / "raw";
			raw.create(TS,proj);
			raw->import_file(TS,read_file("/tmp/scene.nsentry"));
			raw->process_as<nsf::wgeo_v2>(TS);
		};
	}

	if (ImGui::Button("Load wr scenery")) {
		nx << [&](TRANSACT) {
			TS.describe("Load warp room scenery (test)");
			for (auto &&i : util::range(0,16)) {
				nsf::raw_entry::ref raw = ns / "wr-wgeo-$"_fmt(i);
				raw.create(TS,proj);
				raw->import_file(TS,read_file("/tmp/s$.nsentry"_fmt(i)));
				raw->process_as<nsf::wgeo_v2>(TS);
			}
		};
	}

	if (ImGui::Button("Load /tmp/nsentry")) {
		nx << [&](TRANSACT) {
			TS.describe("Load /tmp/nsentry");
			nsf::raw_entry::ref raw = ns / "nsentry";
			raw.create(TS,proj);
			raw->import_file(TS,read_file("/tmp/nsentry"));
		};
	}

	if (ImGui::Button("Load /tmp/nspage")) {
		nx << [&](TRANSACT) {
			TS.describe("Load /tmp/nspage");
			nsf::spage::ref raw = ns / "nspage";
			raw.create(TS,proj);
			raw->import_file(TS,read_file("/tmp/nspage"));
		};
	}

	if (ImGui::Button("Load /tmp/nsfile")) {
		nx << [&](TRANSACT) {
			TS.describe("Load /tmp/nsfile");
			nsf::archive::ref raw = ns / "nsfile";
			raw.create(TS,proj);
			raw->import_file(TS,read_file("/tmp/nsfile"));
			for (misc::raw_data::ref page : raw->get_pages()) {
				if (page->get_data()[2] == 1)
					continue;

				nsf::spage::ref spage = page / "hewm";
				spage.create(TS,proj);
				spage->import_file(TS,page->get_data());
				page->destroy(TS);
				spage->rename(TS,page);
				spage = page;

				for (misc::raw_data::ref pagelet : spage->get_pagelets()) {
					nsf::raw_entry::ref entry = pagelet / "hewmy";
					entry.create(TS,proj);
					entry->import_file(TS,pagelet->get_data());
					pagelet->destroy(TS);
					entry->rename(TS,pagelet);
					entry = pagelet;
					entry->process_by_type(TS,nsf::game_ver::crash2);
				}
			}
		};
	}

	gfx::model::ref smodel = edit::g_selected_asset;

	glPushMatrix();
	glRotatef(edit::g_camera_pitch,1,0,0);
	glRotatef(edit::g_camera_yaw,0,1,0);

	for (gfx::model::ref model : ns.get_asset_names()) {
		if (!model.ok())
			continue;

		auto mesh = model->get_mesh();
		if (!mesh.ok())
			continue;

		auto anim = model->get_anim();
		if (!anim.ok())
			continue;

		auto &&frames = anim->get_frames();
		if (frames.empty())
			continue;

		auto frame = frames[0];
		if (!frame.ok())
			continue;

		glPushMatrix();
		if (smodel.ok()) {
			glTranslatef(-smodel->get_scene_x(),-smodel->get_scene_y(),-smodel->get_scene_z());
		}
		glTranslatef(model->get_scene_x(),model->get_scene_y(),model->get_scene_z());

		auto &&colors = mesh->get_colors();
		auto &&vertices = frame->get_vertices();

		glBegin(GL_TRIANGLES);
		for (auto &&p : mesh->get_triangles()) {
			glColor3ubv(colors[p[0].color_index].v);
			glVertex3fv(vertices[p[0].vertex_index].v);
			glColor3ubv(colors[p[1].color_index].v);
			glVertex3fv(vertices[p[1].vertex_index].v);
			glColor3ubv(colors[p[2].color_index].v);
			glVertex3fv(vertices[p[2].vertex_index].v);
		}
		glEnd();

		glBegin(GL_TRIANGLES);
		for (auto &&p : mesh->get_quads()) {
			glColor3ubv(colors[p[0].color_index].v);
			glVertex3fv(vertices[p[0].vertex_index].v);
			glColor3ubv(colors[p[1].color_index].v);
			glVertex3fv(vertices[p[1].vertex_index].v);
			glColor3ubv(colors[p[2].color_index].v);
			glVertex3fv(vertices[p[2].vertex_index].v);
			glColor3ubv(colors[p[2].color_index].v);
			glVertex3fv(vertices[p[2].vertex_index].v);
			glColor3ubv(colors[p[1].color_index].v);
			glVertex3fv(vertices[p[1].vertex_index].v);
			glColor3ubv(colors[p[3].color_index].v);
			glVertex3fv(vertices[p[3].vertex_index].v);
		}
		glEnd();

		glPopMatrix();
	}

	glBegin(GL_LINE_LOOP);
	glVertex3f(-1,-1,-1);
	glVertex3f(+1,-1,-1);
	glVertex3f(+1,+1,-1);
	glVertex3f(-1,+1,-1);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(-1,-1,+1);
	glVertex3f(+1,-1,+1);
	glVertex3f(+1,+1,+1);
	glVertex3f(-1,+1,+1);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(-1,-1,-1);
	glVertex3f(-1,-1,+1);
	glVertex3f(+1,-1,-1);
	glVertex3f(+1,-1,+1);
	glVertex3f(+1,+1,-1);
	glVertex3f(+1,+1,+1);
	glVertex3f(-1,+1,-1);
	glVertex3f(-1,+1,+1);
	glEnd();

	// Restore the GL matrices.
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

END_MODULE

std::function<void(int)> create_mod_testbox(edit::core &core)
{
	return [m = mod(core)](int delta_time) mutable {
		m.frame(delta_time);
	};
}

}
