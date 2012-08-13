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

#ifndef _ESCAPEMENU_H_
#define _ESCAPEMENU_H_

#include "irrlicht.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;
using namespace io;


//Escape menu class
//pops up when player hits escape
//Is always existing during the game but is invisible unless needed
class CEscapeMenu
{
public:
	//constructor, called once
	CEscapeMenu(irr::IrrlichtDevice *graphics);
	~CEscapeMenu();

	//used only if the thing is visible
	void run();

	void drop();

	//used to show/hide the escape menu
	void setVisible(bool isvisible);
	bool getVisible();


	//used in order to check if the buttons got pressed
	bool getSaveButtonPressed();
	bool getQuitButtonPressed();

private:
	irr::IrrlichtDevice *graphics;

	bool visibility;

	gui::IGUIWindow *escape_window;
	gui::IGUIButton *save_button;
	gui::IGUIButton *quit_button;
};


#endif