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

#ifndef _FADER_H_
#define _FADER_H_

#include "irrlicht.h"


class CFader
{
public:
	CFader(irr::IrrlichtDevice *graphics)
	{
		this->graphics = graphics;
		time = graphics->getTimer()->getTime(); 
		transition_alpha = 0;
		transition_time_start = -1;
	}
	~CFader();
	void update(irr::f32 speed, irr::f32 current_time,bool fadeout)
	{

		float difference = (current_time - transition_time_start)/1000;

		graphics->getVideoDriver()->draw2DRectangle(irr::video::SColor(transition_alpha,0,0,0),
			irr::core::rect<irr::s32>(0,0 ,graphics->getVideoDriver()->getScreenSize().Width, graphics->getVideoDriver()->getScreenSize().Height));
		if(difference >= speed/1000)
		{
		
			if(fadeout==true)
			{
				if(transition_alpha<255)
					transition_alpha++;
			}
			else
			{
				if(transition_alpha>0)
					transition_alpha--;
			}
			transition_time_start = current_time;
		}


	}
	void drop()
	{
		delete this;
	}
private:
	irr::IrrlichtDevice *graphics;
	int time;
	int transition_alpha;
	int transition_time_start;
};

#endif