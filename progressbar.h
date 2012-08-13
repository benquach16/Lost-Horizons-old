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

//CProgressBar
//progressbar.h
//Progress bar class

#ifndef _PROGRESSBAR_H_
#define _PROGRESSBAR_H_

#include "irrlicht.h"
using namespace irr;
using namespace core;
using namespace gui;
using namespace video;

class CProgressBar
{
public:
	CProgressBar(irr::IrrlichtDevice *graphics = (IrrlichtDevice*)0,core::dimension2d<int> pos = core::dimension2d<int>(0,0),core::dimension2d<int> size = core::dimension2d<int>(10,10), int maxvariable=10, gui::IGUIElement *parent =0);
	~CProgressBar();

	void run(int currentvariable);
	void changeMax(int newmax);

private:
	irr::IrrlichtDevice *graphics;
	int max;
	core::dimension2d<int> pos;	//THIS IS THE POSITION THAT IS OFFSET FROM THE PARENT
	core::dimension2d<int> real_pos;	//THIS IS THE ACTUAL POSITION ON THE SCREEN;
	core::vector2d<s32> parent_pos;
	core::dimension2d<int> size;
	gui::IGUIElement *parent;
};



#endif