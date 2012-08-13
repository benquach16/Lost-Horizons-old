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


#ifndef _OPTIONMENU_H_
#define _OPTIONMENU_H_

#pragma once

#include "irrlicht.h"
#include "iostream"
#include "sstream"

using namespace irr;
using namespace core;
using namespace video;
using namespace gui;

//used in the main menu
//displays options to change stuffs
class COptionMenu
{
public:
	COptionMenu(irr::IrrlichtDevice *graphics);
	~COptionMenu();
	void loop();
	void setVisible(bool isvisible)
	{
		visible=isvisible;
	}
	bool getVisible()
	{
		return visible;
	}
	//called by main menu render loop
	int getResolutionX();
	int getResolutionY();
	bool getWindowed();
	bool getHighQuality();
	gui::IGUIButton *getApplyButton();


private:
	bool visible;
	irr::IrrlichtDevice *graphics;
	gui::IGUIElement *control;
	gui::IGUIWindow *window;
	gui::IGUIComboBox *resolutionX;
	gui::IGUIComboBox *resolutionY;
	gui::IGUIComboBox *dynamicLighting;		//to implement
	gui::IGUICheckBox *windowed;
	gui::IGUIComboBox *vsync;				//to implement
	gui::IGUIComboBox *low_fi;
	gui::IGUIButton *quit;
	gui::IGUIButton *apply;
};
#endif