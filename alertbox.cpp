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
#include "alertbox.h"

#define WAITTIME 5000
#define FADETIME 5000;

//displays important text around your ship
CAlertBox::CAlertBox(irr::IrrlichtDevice *graphics)
{
	font = graphics->getGUIEnvironment()->getFont("res/font/astro.xml");
	this->graphics = graphics;
	window_size = graphics->getVideoDriver()->getScreenSize();
	time_start =-1;
}

//main run function
//called from gameloop
void CAlertBox::run()
{
	//loops through annoucements
	//then displays them
	for(int i=0;i<text.size();i++)
	{
		text[i]->setVisible(true);
		float difference = (graphics->getTimer()->getTime() +time_start)/1000;
		text[i]->setOverrideColor(video::SColor(alpha[i],255,255,255));

		//need a better way to move text up
		if(text.size()>1)
		{
			//BUG : above 2 text popups gives me an error about vector out of range
			//text[0]->setRelativePosition(position2di(0,-25));
			int tmp = i*25;
			text[i]->setRelativePosition(position2di(0,tmp));
		}

		if(difference >= 5/1000)
		{

			if(alpha[i]>0)
				alpha[i]--;
			time_start = graphics->getTimer()->getTime();
			
		}

		if(alpha[i]<5)
		{
			text[i]->setVisible(false);
			text[i]->remove();
			text.erase(text.begin()+i);
			alpha.erase(alpha.begin()+i);
		}


	}
}

void CAlertBox::addText(const wchar_t *texttoadd,WARNING_TYPES type)
{
	//add text to the front of the vector
	//so popup changes are easier

	gui::IGUIStaticText *temp = graphics->getGUIEnvironment()->addStaticText(texttoadd,rect<s32>(0,0,window_size.Width,window_size.Height/1.5),false,true);
	temp->setOverrideFont(font);
	temp->setTextAlignment(gui::EGUIA_CENTER,gui::EGUIA_CENTER);
	temp->setVisible(false);
	temp->setEnabled(false);
	if(type==NORMAL)
		temp->setOverrideColor(video::SColor(255,255,255,255));
	if(type==WARNING)
		temp->setOverrideColor(video::SColor(255,255,255,0));
	if(type==ALERT)
		temp->setOverrideColor(video::SColor(255,255,0,0));

	//text.push_back(temp);
	text.insert(text.begin(),temp);
	alpha.insert(alpha.begin(),255);
	//alpha.push_back(255);
}