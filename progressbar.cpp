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

#include "stdafx.h"
#include "progressbar.h"

CProgressBar::CProgressBar(irr::IrrlichtDevice *graphics, core::dimension2d<int> pos, core::dimension2d<int> size, int maxvariable, gui::IGUIElement *parent)
{
	this->graphics = graphics;
	this->max = maxvariable;
	this->pos = pos;
	this->size = size;
	this->parent= parent;
	this->parent_pos = parent->getAbsolutePosition().UpperLeftCorner;

}

void CProgressBar::run(int currentvariable)
{
	//what did i do here
	if(parent->isVisible())
	{
		this->parent_pos = parent->getAbsolutePosition().UpperLeftCorner;
		core::dimension2d<int> temp;
		temp = parent_pos;
		temp += pos;

		core::dimension2d<int> temp_size =temp;
		temp_size+=size;
		rect<s32> temp_rect = rect<s32>(temp.Width,temp.Height,temp_size.Width,temp_size.Height);
		graphics->getVideoDriver()->draw2DRectangle(temp_rect,SColor(128,128,128,128),SColor(128,128,128,128),SColor(128,128,128,128),SColor(128,128,128,128));

		//draw progressbar
		if(currentvariable)
		{
			int bar_percent = currentvariable/max;
			int bar_length = bar_percent * size.Width;
			core::dimension2d<int> bar_temp_size = temp;
			bar_temp_size.Height += size.Height;
			bar_temp_size.Width +=bar_length;
			rect <s32> bar_size = rect<s32>(temp.Width,temp.Height,bar_temp_size.Width,bar_temp_size.Height);

			graphics->getVideoDriver()->draw2DRectangle(bar_size,SColor(128,256,128,128),SColor(128,256,128,128),SColor(128,256,128,128),SColor(128,256,128,128));
		}
	}
}