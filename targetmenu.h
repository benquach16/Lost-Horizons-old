//////////////////////////////////////////////////////////////////////
///Copyright (C) 2011-2012 Benjamin Quach
//
//This file is part of the "Lost Horizons" video game demo
//
//"Lost Horizons" is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
///////////////////////////////////////////////////////////////////////


//targetmenu.h
//gives the players tactical options when attacking a target

#ifndef _TARGETMENU_H_
#define _TARGETMENU_H_

#include "irrlicht.h"
#include "ship.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace gui;

class CTargetMenu
{
public:
	CTargetMenu(irr::IrrlichtDevice *graphics);
	~CTargetMenu();

	void loop(CShip *player_target);

	void drop();
	void setVisible(bool isvisible);
	bool getVisible();
	int getSelected();
private:
	bool visibility;

	gui::IGUIWindow *window;
	gui::IGUIStaticText *ship_name;
	gui::IGUIStaticText *ship_class;
	gui::IGUIStaticText *ship_description;
	gui::IGUIStaticText *target_subsystem;
	gui::IGUIListBox *subsystems;
	gui::IGUIStaticText *subsystem_health;


	irr::IrrlichtDevice *graphics;
	dimension2df screen_size;
	rect<s32> window_size;

	int tmp;
	
};

#endif