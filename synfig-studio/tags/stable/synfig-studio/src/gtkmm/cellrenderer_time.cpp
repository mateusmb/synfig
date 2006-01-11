/* === S Y N F I G ========================================================= */
/*!	\file cellrenderer_time.cpp
**	\brief Template File
**
**	$Id: cellrenderer_time.cpp,v 1.1.1.1 2005/01/07 03:34:35 darco Exp $
**
**	\legal
**	Copyright (c) 2002-2005 Robert B. Quattlebaum Jr., Adrian Bentley
**
**	This package is free software; you can redistribute it and/or
**	modify it under the terms of the GNU General Public License as
**	published by the Free Software Foundation; either version 2 of
**	the License, or (at your option) any later version.
**
**	This package is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
**	General Public License for more details.
**	\endlegal
*/
/* ========================================================================= */

/* === H E A D E R S ======================================================= */

#ifdef USING_PCH
#	include "pch.h"
#else
#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include <gtkmm/entry.h>
#include <gtkmm/button.h>
#include "cellrenderer_time.h"
#include "app.h"
#include "widget_time.h"

#endif

/* === U S I N G =========================================================== */

using namespace std;
//using namespace etl;
using namespace synfig;
using namespace studio;

/* === M A C R O S ========================================================= */

#if ! defined(_)
#define _(x)	(x)
#endif

/* === G L O B A L S ======================================================= */

/* === P R O C E D U R E S ================================================= */

/* === M E T H O D S ======================================================= */

CellRenderer_Time::CellRenderer_Time():
	Glib::ObjectBase	(typeid(CellRenderer_Time)),
	Gtk::CellRendererText	(),
	property_time_(*this,"time",synfig::Time(0)),
	property_fps_(*this,"fps", float(0))
{
	CellRendererText::signal_edited().connect(sigc::mem_fun(*this,&studio::CellRenderer_Time::string_edited_));
}

CellRenderer_Time::~CellRenderer_Time()
{
	synfig::info("CellRenderer_Time::~CellRenderer_Time(): deleted");
}

void
CellRenderer_Time::string_edited_(const Glib::ustring&path,const Glib::ustring&str)
{
	signal_edited_(path,Time((String)str,(Real)Time(property_fps_)));
}

void
CellRenderer_Time::render_vfunc(
		const Glib::RefPtr<Gdk::Drawable>& window,
		Gtk::Widget& widget,
		const Gdk::Rectangle& background_area,
		const Gdk::Rectangle& ca,
		const Gdk::Rectangle& expose_area,
		Gtk::CellRendererState flags)
{
	if(!window)
		return;
	//int	height = ca.get_height();

	Gtk::StateType state = Gtk::STATE_INSENSITIVE;
	if(property_editable())
		state = Gtk::STATE_NORMAL;
	if((flags & Gtk::CELL_RENDERER_SELECTED) != 0)
		state = (widget.has_focus()) ? Gtk::STATE_SELECTED : Gtk::STATE_ACTIVE;

	const Time time(property_time_);
	const float fps((Real)Time(property_fps_));
	
	property_text()=(Glib::ustring)time.get_string(fps,App::get_time_format());

	CellRendererText::render_vfunc(window,widget,background_area,ca,expose_area,flags);
}


Gtk::CellEditable*
CellRenderer_Time::start_editing_vfunc(
	GdkEvent* event,
	Gtk::Widget& widget,
	const Glib::ustring& path,
	const Gdk::Rectangle& background_area,
	const Gdk::Rectangle& cell_area,
	Gtk::CellRendererState flags)
{
	// If we aren't editable, then there is nothing to do
	if(!property_editable())
		return 0;

	const Time time(property_time_);
	const float fps((Real)Time(property_fps_));
	
	property_text()=(Glib::ustring)time.get_string(fps,App::get_time_format()|Time::FORMAT_FULL);
#if 0
	Widget_Time* widget_time(manage(new Widget_Time));
	widget_time->set_fps(fps);
	widget_time->set_value(time);
	widget_time->signal_editing_done().connect(sigc::mem_fun(*this, &CellRenderer_Time::on_value_editing_done));
	return widget_time;
#else
	return CellRendererText::start_editing_vfunc(event,widget,path,background_area,cell_area,flags);
#endif
}
