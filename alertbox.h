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


//alertbox.h
//Used to show text alerts onscreen that disappear after a certain amount of time

#ifndef _ALERTBOX_H_
#define _ALERTBOX_H_

#include "irrlicht.h"

#include "vector"

using namespace irr;
using namespace core;
using namespace gui;
using namespace video;

enum WARNING_TYPES
{
	NORMAL=0,
	WARNING=1,
	ALERT=2
};

class CAlertBox 
{
public:
	CAlertBox(irr::IrrlichtDevice *graphics);
	void run();
	void addText(const wchar_t *texttoadd,WARNING_TYPES type);
	~CAlertBox();

private:
	gui::IGUIFont *font;
	std::vector<gui::IGUIStaticText*> text;
	std::vector<int> alpha;
	irr::IrrlichtDevice *graphics;

	core::dimension2d<s32> window_size;
	int time_start;
};

#endif